#ifndef TRR_RESOURCE_MANAGER_TYPEDEF_HPP
#define TRR_RESOURCE_MANAGER_TYPEDEF_HPP

#include <Core/ContentHandler/ResourceManager.hpp>

/* Include headers for loaders here */
#include <Core/ContentHandler/Loaders/TestLoader.hpp>
#include <Core\ContentHandler\Loaders\RawDataLoader.hpp>
namespace trr
{
	typedef ResourceManager<
	
		/* List loader classes here */

		TestLoader,
		RawDataLoader		// raw char buffer


	
	
	> ResourceManagerImpl;
}

#endif