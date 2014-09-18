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
		bool temp = internal_init( size_in_bytes );
		EXIT_CRITICAL_SECTION;
		return temp;
	}

	void* FlatAllocate(uint size_in_bytes, uint alignment = 2)
	{
		ENTER_CRITICAL_SECTION;
		void* temp = internal_allocate( size_in_bytes, alignment );
		EXIT_CRITICAL_SECTION;
		return temp;
	}
	
	void deallocate(void* mem)
	{
		ENTER_CRITICAL_SECTION;
		internal_deallocate( mem );
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
		int temp = internal_getMemoryUtilization();
		EXIT_CRITICAL_SECTION;
		return temp;
	}
	
	void destroy()
	{
		ENTER_CRITICAL_SECTION;
		internal_destroy();
		EXIT_CRITICAL_SECTION;
#ifdef USE_CRITICAL_SECTION_LOCK
		DeleteCriticalSection(&CriticalSection);
#endif
	}	
};