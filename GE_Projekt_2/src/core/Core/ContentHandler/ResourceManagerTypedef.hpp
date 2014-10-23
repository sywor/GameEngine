#ifndef TRR_RESOURCE_MANAGER_TYPEDEF_HPP
#define TRR_RESOURCE_MANAGER_TYPEDEF_HPP

#include <Core/ContentHandler/ResourceManager.hpp>

/* Include headers for loaders here */
#include <Core/ContentHandler/Loaders/TestLoader.hpp>
#include <Core/ContentHandler/Loaders/RawDataLoader.hpp>

namespace trr
{
	typedef ResourceManager< 

		/* Block size of the internal memory allocator */
		8,

		/* Size of the internal memory allocator */
		524288,

		/* Size of internal memory for metadata */
		262144,

		/* List loader classes here */

		TestLoader,
		RawDataLoader		// raw char buffer


	
	
	> ResourceManagerImpl;
}

#endif