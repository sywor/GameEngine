#pragma once
#include "Allocation.h"

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
		T* object = reinterpret_cast<T*>(new(FlatAllocate(sizeof(T)))T(args...));

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
