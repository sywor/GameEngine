#ifndef RESOURCE_LOADER_H
#define RESOURCE_LOADER_H

#include "Resource.hpp"

#include <string>
#include <cstdint>
#include <Core/ContentHandler/ContainerLoaders/DataContainer.hpp>
#include <Core/Memory/Allocation.h>
#include <Core/Memory/PoolAllocator.h>

namespace trr
{
	
	class ResourceLoader abstract
	{
	private:

		/*
			Specific function for loading implementation.
			Note: Should the memory allocator run out of memory
				CONTENT_CALLBACK_OUT_OF_MEMORY should be returned 
				in order to propagate the correct error. 
				If ignored and nullptr is used when an error occurrs,
				CONTENT_CALLBACK_LOADING_FAILED will be propagated insted.
		*/
		virtual void* internal_Load( DataContainer in ) = 0;
		virtual void internal_unload( void* data) = 0;

	public:
		virtual ~ResourceLoader();

		/*
			Loopthrough function for the asynchronous interface.
			May be called by multiple threads at the same time.
		*/
		void* Load( DataContainer _data);

		/*
			Loopthrough function for the asynchronous interface.
			May be called by multiple threads at the same time.
		*/
		void Unload( void* data );

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