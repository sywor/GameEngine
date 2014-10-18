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
	uint32_t	end;
	//Marker*		top;

	int n;
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
#ifdef _DEBUG
		assert((marker + sizeof(T)) <= end);
#endif
		uint32_t dat = (uint32_t)data + size;
		if ((marker + sizeof(T)) > end)
		{
			assert("outside mem bound");
				return nullptr;
		}
			

		return reinterpret_cast<T*>(new(FlatAllocate(sizeof(T)))T(args...));
	}

	template<typename T>
	void deallocate(T*& obj)
	{
		uint32_t objAddr = (uint32_t)obj;

		// the object we're trying to remove is not the head
		//assert((uint32_t)obj == (marker)-sizeof(T));
		
		//obj->~T();
		obj	= nullptr;
		marker	= objAddr;
	}

	void		setMarker(const uint32_t &_marker) { marker = _marker; }
	uint32_t	getMarker();
	void		clear();
};
