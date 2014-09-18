#include "DefaultAllocator.h"

#include <memory>
#include <assert.h>



bool DefaultAllocator::internal_init(uint size_in_bytes)
{
	return true;
}

void* DefaultAllocator::internal_allocate(uint size_in_bytes, uint alignement)
{
	return new char[size_in_bytes]();
}

void DefaultAllocator::internal_deallocate(void* mem)
{
	delete mem;
}

uint DefaultAllocator::internal_getSize()
{
	return size;
}

