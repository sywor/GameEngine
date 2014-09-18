#pragma once
#include "Allocation.h"

typedef unsigned int uint;


class DefaultAllocator	: public Allocator
{
private:

protected:
	virtual void	internal_deallocate(void* mem = nullptr)					override;
public:
	DefaultAllocator() {}
	~DefaultAllocator(){ destroy(); }
	virtual bool	internal_init(uint size_in_bytes)							override;
	virtual void*	internal_allocate(uint size_in_bytes, uint alignment = 2)	override;
	
	virtual uint	internal_getSize()											override;

	template<typename T, typename... S>
	T* allocate(S... args)
	{ 
		return new T( args... );
	}

	template<typename T>
	void deallocate(T*& obj)
	{
		delete obj;
	}
};
