#pragma once
#include "Allocation.h"
#include "StackAllocator.h"


typedef unsigned int uint;



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

		return reinterpret_cast<T*>(new(FlatAllocate(sizeof(T), alignment)) T(args...));
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

