#include "Allocation.h"

#include <memory>
#include <assert.h>
/* Allocator */
Allocator::Allocator()
{
	data = nullptr;
}

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

	top = new((void*)((uint32_t)(data)+size-sizeof(Marker)))Marker;
	top->address = marker;

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
	printf("dealloc:%p\n",mem);
	marker = (uint32_t)mem;
}

uint StackAllocator::internal_getSize()
{
	return size;
}

int32_t	StackAllocator::getMarker()
{
	return marker;
}

void StackAllocator::clear()
{
	marker = (int32_t)data;
}

void StackAllocator::pop()
{
	// error handling goes here

	marker	= top->address;				// link marker to the the next object in line
	top->destructorCall((void*)(marker));	// call destructor
	top		= top + sizeof(Marker);		// increase top address
}

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

bool PoolAllocator::internal_init( uint32_t size_in_bytes )
{
	data = new uint8_t[size_in_bytes];	
	if(!data)
		return false;

	size = size_in_bytes;
	openList.push_back( MemIndex( data, size_in_bytes ) );
	return true;
}

void PoolAllocator::checkMerge(int i)
{
	auto& list( openList );
	if( i + 1 > list.size() )
		return;
	if( list[i].size == list[i+1].size )
	{
		if( (uint)list[i].memory + (uint)list[i].size == (uint)list[i+1].memory )
		{
			// IF ADD ALIGN CHANGE THIS PART
			if( !(((uint)list[i].memory - (uint)data) % (list[i].size*2)) )
			{
				MemIndex t( list[i].memory, list[i].size * 2 );
				list.erase( list.begin() + i );
				list.erase( list.begin() + i );
				insertOpenList( t );
			}
			else if( i - 1 > 0 && list[i-1].size == list[i].size )
			{
				if( (uint)list[i-1].memory + (uint)list[i].size == (uint)list[i].memory )
				{
					MemIndex t( list[i-1].memory, list[i].size * 2 );
					list.erase( list.begin() + i - 1 );
					list.erase( list.begin() + i - 1 );
					insertOpenList( t );
				}
			}
		}
	}
	return;
}

int PoolAllocator::insertOpenList( MemIndex t )
{
	auto& list( openList );
	for( int i(0); i < list.size(); i++ )
	{
		if( list[i].size >= t.size )
		{
			if( list[i].size == t.size && list[i].memory >= t.memory )
			{
				i++;
				for( ; i < list.size(); i++ )
					if( list[i].size < t.size || list[i].memory > t.memory )
						break;
			}

			if( i >= list.size() )
				break;

			list.insert( list.begin() + i, t );
			checkMerge( i );
			return i;
		}
	}

	list.push_back( t );
	return list.size() - 1;
}
int PoolAllocator::insertClosedList( MemIndex t )
{

	closedList.push_back( t );

	return closedList.size() - 1;
}

void* PoolAllocator::closeMemory( int i, uint size_in_bytes )
{
	if( openList[i].size / 2 < size_in_bytes || openList[i].size / 2 < block_size )
	{
		void* t = openList[i].memory;
		insertClosedList( openList[i] );
		openList.erase( openList.begin() + i );
		return t;
	}
	else
	{
		MemIndex t = openList[i];

		openList.insert( openList.begin() + i, MemIndex( (char*)t.memory + (t.size / 2), t.size / 2 ) );
		openList.insert( openList.begin() + i, MemIndex( t.memory, t.size / 2 ) );

		openList.erase( openList.begin() + i + 2 );
		return closeMemory( i, size_in_bytes );
	}

	// cast error: out of memory for supplied size... ? 

	return nullptr;
}

void* PoolAllocator::internal_allocate( uint size_in_bytes, uint alignement )
{
	size_in_bytes = size_in_bytes % 2 > 0 ? (size_in_bytes - size_in_bytes % 2) * 2: size_in_bytes;
	int nrChunks = openList.size();
	for( int i = 0; i < nrChunks; i++ )
	{
		if( openList[i].size < size_in_bytes )
			continue;
		else
			return closeMemory( i, size_in_bytes );
	}
	return nullptr;
}

void PoolAllocator::internal_deallocate( void* mem )
{
	for( int i(0); i < closedList.size(); i++ )
	{
		if( closedList[i].memory == mem )
		{
			insertOpenList( closedList[i] );
			closedList.erase( closedList.begin() + i );
		}
	}
}

uint PoolAllocator::internal_getSize()
{
	return size;
}

void PoolAllocator::internal_destroy()
{
	if(data)
		free(data);

	data = nullptr;
}

int	PoolAllocator::internal_getMemoryUtilization()
{
	int opensize = openList.size();
	int freeMem = 0;
	for( int i = 0; i < opensize; i++ )
	{
		freeMem += openList[i].size;
	}

	return 100 - ((float)freeMem / size) * 100;
}