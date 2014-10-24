#include "PoolAllocator.h"

#include <memory>
#include <assert.h>

bool PoolAllocator::internal_init( uint32_t size_in_bytes )
{
	openList.reserve( 1000 );
	closedList.reserve( 1000 );

	data = new uint8_t[size_in_bytes];	
	if(!data)
		return false;

	size = size_in_bytes;
	openList.push_back( MemIndex( data, size_in_bytes ) );
	return true;
}

void PoolAllocator::checkMerge( int i )
{
	auto& list( openList );
	if( i + 1 > list.size() )
		return;
	if( list[i].size != list[i+1].size && (int)i - 1 >= 0 && list[i-1].size == list[i].size )
		i--;
	if( list[i].size == list[i+1].size )
	{
		if( (uint)list[i].memory + (uint)list[i].size * block_size == (uint)list[i+1].memory )
		{
			// IF ADD ALIGN CHANGE THIS PART
			if( !(((uint)list[i].memory - (uint)data) % (list[i].size*2)) )
			{
				MemIndex t( list[i].memory, list[i].size * 2 );
				list.erase( list.begin() + i );
				list.erase( list.begin() + i );
				insertOpenList( t );
			}
			else if( i - 1 > 0 && list[i - 1].size == list[i].size) 
			{
				if ((uint)list[i - 1].memory + (uint)list[i].size == (uint)list[i].memory)
				{
					MemIndex t(list[i - 1].memory, list[i].size * 2);
					list.erase(list.begin() + i - 1);
					list.erase(list.begin() + i - 1);
					insertOpenList(t);
				}
			}
		}
	}
	return;
}

int PoolAllocator::insertOpenList( MemIndex t )
{
	auto& list( openList );

#define OLD_CODE
#ifndef OLD_CODE
	size = list.size();
	int min = 0, max = size;
	for (int i(size / 2);;)
	{
		if (i == 0)
		{
			list.insert(list.begin() + i, t);
			checkMerge(i);
			return i;
		}
		if (i == list.size())
		{
			list.push_back( t );
			return list.size() - 1;
		}
		if (max - min == 1)
		{
			list.insert(list.begin() + i, t);
			checkMerge(i);
			return i;
		}

		if (list[i].size == t.size)
		{
			if (list[i].memory == t.memory)
			{
			}
			else if (list[i].memory > t.memory)
			{
				i++;
				for (; i < size; i++)
					if (list[i].size < t.size || list[i].memory > t.memory)
						break;
			}
			else if (list[i].memory < t.memory)
			{
				i--;
				for (; i > 0; i--)
					if (list[i].size > t.size || list[i].memory < t.memory)
						break;
			}

			if (i >= size)
				break;

			list.insert(list.begin() + i, t);
			checkMerge(i);
			return i;
		}
		else if (list[i].size > t.size)
		{
			max = i;
			i -= int(float(i-min) / 2 + 0.5f);
		}
		else
		{
			min = i;
			i += (max - i) / 2;
		}
	}
#endif

#ifdef OLD_CODE
	for (unsigned int i(0); i < list.size(); i++)
	{
		if( list[i].size >= t.size )
		{
			if( list[i].size == t.size )
			{
				if( t.memory >= list[i].memory)
				{
					i++;
					for( ; i < list.size(); i++ )
						if( t.size < list[i].size )
							break;
						else if( t.memory > list[i].memory )
						{
							i--;
							break;
						}
				}
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
#endif
}
int PoolAllocator::insertClosedList( MemIndex t )
{

	closedList.push_back( t );

	return closedList.size() - 1;
}

void* PoolAllocator::closeMemory(unsigned int i, uint size_in_bytes)
{
	if (openList[i].size / 2 < size_in_bytes || openList[i].size / 2 < block_size)
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
	for (unsigned int i(0); i < closedList.size(); i++)
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