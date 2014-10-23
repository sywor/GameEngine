#ifndef TRR_RESOURCE_MANAGER_TYPEDEF_HPP
#define TRR_RESOURCE_MANAGER_TYPEDEF_HPP

#include <Core/ContentHandler/ResourceManager.hpp>

/* Include headers for loaders here */
#include <Core/ContentHandler/Loaders/TestLoader.hpp>
#include <Core/ContentHandler/Loaders/RawDataLoader.hpp>
#include <Core/ContentHandler/Loaders/RawImageLoader.h>


namespace trr
{
	typedef ResourceManager<

		/* block size of the internal memory allocator */
		8,

		/* Size of the internal memory allocator */
		1024,

		/* List loader classes here */
		RawImageLoader,
		TestLoader,
		RawDataLoader		// raw char buffer


	
	
	> ResourceManagerImpl;
}

#endif