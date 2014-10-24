#pragma once
#include "Allocation.h"


class PoolAllocator		: public Allocator
{
private:
	uint block_size;
	
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
	void checkMerge( int i);
	int insertOpenList( MemIndex t );
	int insertClosedList( MemIndex t );
	void* closeMemory(unsigned int i, uint size_in_bytes);

	virtual bool	internal_init(uint32_t size_in_bytes)						override;
	virtual void*	internal_allocate(uint size_in_bytes, uint alignement = 2)	override;
	virtual void	internal_deallocate(void* mem)								override;
	virtual uint	internal_getSize()											override;
	virtual void	internal_destroy()											override;
	virtual int		internal_getMemoryUtilization()								override;

public:
	PoolAllocator( int t_block_size, int size_in_bytes )
	{
		block_size = t_block_size; 
		init( size_in_bytes );
	}
	virtual ~PoolAllocator()
	{ internal_destroy(); }	

	template<typename T, typename... S>
	T* allocate(S... args)
	{
		void* pointer = FlatAllocate(sizeof(T));
		if( pointer != nullptr )
			return reinterpret_cast<T*>( new( pointer )T(args...)); 
		return nullptr;

	}
};

