#include "AlignedStackAllocator.h"

#include <memory>
#include <assert.h>


/* Aligned StackAllocator */
void AlignedStackAllocator::internal_destroy()
{
	if(data)
	{
		int adjust = *(int*)(((char*)data)-4);
		free((void*)(((char*)data) - adjust));
	}
	data = nullptr;
}

bool AlignedStackAllocator::internal_init(uint _size_in_bytes)
{
	void* origin		= malloc(_size_in_bytes + alignment);
	int adjust			= alignment - (int(origin) & 0x0f);
	char* aligned		= (char*)origin + adjust;
	*((int*)(aligned-4))= adjust;
	data				= aligned;

	if(!data)
		return false;

	marker	= (int32_t)data;
	size	= _size_in_bytes;

	top = new((void*)((uint32_t)data-sizeof(Marker)))Marker;
	top->address = marker;

	return true;
}


void* AlignedStackAllocator::internal_allocate(uint size_in_bytes, uint _alignment)
{
	/*size_in_bytes fcuks alignment for differently sized objects - fix! */
	int adjust			= alignment - ( (marker + size_in_bytes) & 0x0f);
	char* aligned		= (char*)marker + adjust;
	*((int*)(aligned-4))= adjust;
	void* mem			= aligned;
	//marker = (uint32_t)mem;
	printf("addr:%p\n",mem);
	
	marker = (uint32_t)aligned + size_in_bytes;
	top = new(top-sizeof(Marker))Marker;
	top->address = marker;
	return mem;

}

void AlignedStackAllocator::internal_deallocate(void* mem)
{

}

int32_t	AlignedStackAllocator::getMarker()
{
	return marker;
}

void AlignedStackAllocator::clear()
{
	marker = (int32_t)data;
}

uint AlignedStackAllocator::internal_getSize()
{
	return size;
}
