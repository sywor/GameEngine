#pragma once

#define CRITICAL_SECTION_FAILED_INIT 0x00000400

#include <cstdint>
#include <vector>
#include <assert.h>
#include <Windows.h>
#include <mutex>

#define USE_ASYNC_LOCKING
#define USE_CRITICAL_SECTION_LOCK

#ifdef USE_LOCKING
	#ifdef USE_CRITICAL_SECTION_LOCK
		#define ENTER_CRITICAL_SECTION EnterCriticalSection(&CriticalSection)
		#define EXIT_CRITICAL_SECTION LeaveCriticalSection(&CriticalSection)
	#else
		#define ENTER_CRITICAL_SECTION mutex.lock()
		#define EXIT_CRITICAL_SECTION mutex.unlock()
	#endif
#else
#define ENTER_CRITICAL_SECTION 
#define EXIT_CRITICAL_SECTION 
#endif


typedef unsigned int uint;

struct Marker
{
	uint32_t address;
	void(*destructorCall)(void* pMem);
};

template<typename T>
void CallDestructor(void* pointer)
{
	static_cast<T*>(pointer)->~T();
}

class Allocator
{
private:
protected:
	uint32_t	size;
	void*		data;

#ifdef USE_CRITICAL_SECTION_LOCK
	CRITICAL_SECTION CriticalSection; 
#else
	std::mutex mutex;
#endif

	virtual bool	internal_init(uint32_t size_in_bytes)						= 0;
	virtual void*	internal_allocate(uint size_in_bytes, uint alignment = 2)	= 0;
	virtual void	internal_deallocate(void* mem)								= 0;
	virtual uint	internal_getSize()											= 0;
	virtual int		internal_getMemoryUtilization() { return 0; }
	virtual void	internal_destroy(){}

public:
	Allocator();

	virtual ~Allocator()
	{ destroy(); }

	bool init(uint32_t size_in_bytes)
	{
#ifdef USE_CRITICAL_SECTION_LOCK
		if (!InitializeCriticalSectionAndSpinCount(&CriticalSection, CRITICAL_SECTION_FAILED_INIT ) ) 
			return false;
#endif

		ENTER_CRITICAL_SECTION;
		bool temp = init( size_in_bytes );
		EXIT_CRITICAL_SECTION;
		return temp;
	}

	void* allocate(uint size_in_bytes, uint alignment = 2)
	{
		ENTER_CRITICAL_SECTION;
		void* temp = allocate( size_in_bytes, alignment );
		EXIT_CRITICAL_SECTION;
		return temp;
	}
	
	void deallocate(void* mem)
	{
		ENTER_CRITICAL_SECTION;
		deallocate( mem );
		EXIT_CRITICAL_SECTION;
	}

	uint getSize()
	{
		ENTER_CRITICAL_SECTION;
		uint temp = getSize();
		EXIT_CRITICAL_SECTION;
		return temp;
	}
	
	int	getMemoryUtilization() 
	{
		ENTER_CRITICAL_SECTION;
		int temp = getMemoryUtilization();
		EXIT_CRITICAL_SECTION;
		return temp;
	}
	
	void destroy()
	{
		ENTER_CRITICAL_SECTION;
		destroy();
		EXIT_CRITICAL_SECTION;
#ifdef USE_CRITICAL_SECTION_LOCK
		DeleteCriticalSection(&CriticalSection);
#endif
	}	
};

class StackAllocator	
	: public Allocator
{
private:
	int32_t		marker;
	Marker*		top;

protected:
	virtual void	internal_deallocate(void* mem = nullptr)					override;
public:
	~StackAllocator(){ destroy(); }
	virtual bool	internal_init(uint size_in_bytes)		override;
	virtual void*	internal_allocate(uint size_in_bytes, uint alignment = 2)	override;
	
	virtual uint	internal_getSize()											override;
	virtual void	internal_destroy()											override;

	template<typename T, typename... S>
	T* allocate(S... args)
	{ 
		assert(marker + sizeof(T) <= (uint32_t)top);

		// the header object
		// constructed first since we need to store address for previous object
		top					= new(top-sizeof(Marker))Marker;
		top->address		= marker;
		top->destructorCall = &CallDestructor<T>;

		// the allocated object
		// increases marker by size of object
		T* object = reinterpret_cast<T*>(new(allocate(sizeof(T)))T(args...)); 

		return object;
	}

	template<typename T>
	void deallocate(T*& obj)
	{
		uint32_t objAddr = marker - sizeof(T);

		assert(objAddr == (uint32_t)obj);
		assert(objAddr <= ((uint32_t)data + size) );
		assert((uint32_t)data <= objAddr);
		
		obj->~T();
		obj		= nullptr;
		marker	= objAddr;

		sizeAllocations -= sizeof(T);
		//numAllocations--;
	}

	void		pop();
	int32_t		getMarker();
	void		clear();
};

class AlignedStackAllocator		
	: public Allocator
{
private:
	int32_t		marker, alignment;
	Marker*		top;
protected:
	virtual void	internal_deallocate(void* mem)								override;
public:
	~AlignedStackAllocator(){ destroy(); }
	virtual bool	internal_init(uint size_in_bytes)							override;
	virtual void*	internal_allocate(uint size_in_bytes, uint align = 2)		override;
	
	virtual uint	internal_getSize()											override;
	virtual void	internal_destroy()											override;

	template<typename T, typename... S>
	T* allocate(S... args)
	{ 
		assert(marker <= ((uint32_t)data + size) );

		return reinterpret_cast<T*>( new(allocate(sizeof(T), alignment)) T(args...));
	}

	template<typename T>
	void deallocate(T*& obj)
	{
		int adjust	= alignment - ( (marker + sizeof(T)) & 0x0f);
		marker		= marker -sizeof(T) - adjust;
		obj->~T();
		obj		= nullptr;

		sizeAllocations -= sizeof(T);	// origin - (marker+sizeT+offset(?))
		numAllocations--;
	}

	void pop();

	int32_t		getMarker();
	void		clear();

};


class PoolAllocator		: public Allocator
{
private:
	int block_size;
	
	struct MemIndex
	{
		MemIndex( void* data, uint32_t byteSize )
			: memory( data ), size( byteSize )
		{
		
		}

		void* memory;
		uint32_t size;
	};
	
	std::vector< MemIndex > openList, closedList;


protected:
	void checkMerge( int i );
	int insertOpenList( MemIndex t );
	int insertClosedList( MemIndex t );
	void* closeMemory( int i, uint size_in_bytes );

	virtual bool	internal_init(uint32_t size_in_bytes)						override;
	virtual void*	internal_allocate(uint size_in_bytes, uint alignement = 2)	override;
	virtual void	internal_deallocate(void* mem)								override;
	virtual uint	internal_getSize()											override;
	virtual void	internal_destroy()											override;
	virtual int		internal_getMemoryUtilization()								override;

public:
	PoolAllocator( int t_block_size ){ block_size = t_block_size; }
	virtual ~PoolAllocator()
	{ internal_destroy(); }	

	template<typename T, typename... S>
	T* allocate(S... args)
	{
		void* pointer = allocate(sizeof(T)); 
		if( pointer != nullptr )
			return reinterpret_cast<T*>( new( pointer )T(args...)); 
		return nullptr;

	}
};

