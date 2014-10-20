#ifndef RESOURCE_LOADER_H
#define RESOURCE_LOADER_H

#include "Resource.hpp"

#include <string>
#include <cstdint>
#include <Core/ContentHandler/DataContainer.hpp>
#include <Core/Memory/Allocation.h>
#include <Core/Memory/PoolAllocator.h>

namespace trr
{
	
	class ResourceLoader abstract
	{
	private:

		virtual void* internal_Load( DataContainer in ) = 0;
		virtual void internal_unload(void** data) = 0;

	public:
		virtual ~ResourceLoader();

		/*
			Loopthrough function for the asynchronous interface.
			Will check hash reference count before and after loading 
			in order to counter chained loading and unloading.
		*/
		void* Load( DataContainer _data);

		/*
			Loopthrough function for the asynchronous interface.
			Will check hash reference count before and after loading 
			in order to counter chained loading and unloading.
		*/
		void Unload(Resource& r);

		/*
			Get the file extension associated with a particular
			resource loader. Used for matching a type of resource
			with a loader.
		*/
		virtual const std::string GetExtension() = 0;

		/**/
		static void SetAllocator(PoolAllocator* allocator)	{ ResourceLoader::m_pAllocator = allocator;	}

	private:

	protected:
		static PoolAllocator* m_pAllocator;
	};

}


#endif