#include "StackAllocator.h"

#include <memory>
#include <assert.h>


/* StackAllocator */
void StackAllocator::internal_destroy()
{
	if(data)
		free(data);
	
	data = nullptr;
}

bool StackAllocator::internal_init(uint size_in_bytes)
{
	//data = new uint8_t[size_in_bytes];	
	data = malloc(size_in_bytes);
	if(!data)
		return false;
	
	marker  = (uint32_t)data;
	size	= size_in_bytes;
	end		= (uint32_t)marker + size;
	//top = new((void*)((uint32_t)(data)+size-sizeof(Marker)))Marker;
	//top->address = marker;

	//sizeAllocations = 0;
	//numAllocations = 0;
	return true;
}

void* StackAllocator::internal_allocate(uint size_in_bytes, uint alignement)
{
	void* mem	= (void*)marker;
	marker		= marker + size_in_bytes;

	return mem;
}

void StackAllocator::internal_deallocate(void* mem)
{
	marker = (uint32_t)mem;
}

uint StackAllocator::internal_getSize()
{
	return size;
}

uint32_t StackAllocator::getMarker()
{
	return marker;
}

void StackAllocator::clear()
{
	marker = (uint32_t)data;
}

//void StackAllocator::pop()
//{
//	// error handling goes here
//
//	marker	= top->address;				// link marker to the the next object in line
//	top->destructorCall((void*)(marker));	// call destructor
//	top		= top + sizeof(Marker);		// increase top address
//}
