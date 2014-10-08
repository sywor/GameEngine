#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "ResourceLoader.hpp"
#include "Resource.hpp"
#include "TemplateMagic.hpp"

#include "../Memory/DefaultAllocator.h"

#include <cstdint>
#include <array>
#include <vector>
#include <string>
#include <functional>

static bool MatchExtension(const char* fExt, const char *lExt)
{
	while (*lExt)
	{
		if (*lExt == ('?'))
		{
			if (!*fExt)
				return false;

			++fExt;
			++lExt;
		}
		else if (*lExt == ('*'))	// Wildcard bitches!
		{
			if (MatchExtension(fExt, lExt + 1))
				return true;
			if (*fExt && MatchExtension(fExt + 1, lExt))
				return true;

			return false;
		}
		else
		{
			if (::CharUpper(MAKEINTRESOURCE(MAKELONG(*fExt++, 0))) != ::CharUpper(MAKEINTRESOURCE(MAKELONG(lExt++, 0))))
				return false;
		}
	}
	return !*fExt && !*lExt;
}

namespace trr
{

	template< typename... LoadersDef >
	class ResourceManager
	{
	private:

	public: 

		// ///////////////////////////////////////////////////////////////////////////////// 
		// class utility
		ResourceManager()
		{
			for( int i = 0; i < sizeof...(LoadersDef); i++ )
			{
				loaders[ i ] = nullptr;
			}
			InitializeArrayWithNew< LoadersDef... >( (int**)&loaders[0], &contentAllocator );
		}

		~ResourceManager()
		{
			for( int i = 0; i < sizeof...(LoadersDef); ++i )
			{
				if( loaders[ i ] != nullptr )
					contentAllocator.deallocate( loaders[i] );
				loaders[ i ] = nullptr;
			}
		}


		// ///////////////////////////////////////////////////////////////////////////////// 
		// content interface

		/*
			Attempts to load asset and returns void* to 
			respective result container associated with the sought loader.
		*/
		void* GetResource(std::string path)
		{
			// implement proper hash!!!
			int hash = std::atoi(path.c_str());

			// return asset if already loaded
			for (Resource& asset : assetList)
			{
				if (asset.hash == hash)
				{
					asset.nrReferences++;
					return asset.data;
				}
			}
			
			// find compatible loader to new resource
			std::size_t sep = path.find_last_of(".");
			const std::string ext = path.substr(sep+1);
			for (int i = 0; i < loaders.size(); ++i)
			{
				if (MatchExtension(ext.c_str(), loaders[i]->GetExtension().c_str()))
				{
					Resource r;
					r.hash = hash;
					r.nrReferences++;
					r.loaderIndex = i;
					loaders[i]->Load(path, r);
					
					assetList.push_back(r);

					break;
				}
			}
			// debug only, fix!!!

			if (assetList.size() > 0)
				return assetList[0].data;
			else return nullptr;
		}

		/*
			Attempts to load asset and call the supplied function once loaded.
			data-pointer is pointer of the result container associated with the sough loader.
		*/
		void GetResource( std::string path, std::function<void(void* data)> callback );

		/*
			Will decrease the referense counter of the supplied hash
			and add job to unload should the number of referenses reach zero.
		*/
		void Unload(std::uint64_t handle)
		{
			for (Resource& asset : assetList)	// find resource
			{
				if (asset.hash == handle)
				{
					if (asset.loaderIndex < loaders.size())	// find associated unloader
					{
						loaders[asset.loaderIndex]->Unload(asset);
					}
				}
			}
		}


		/*
			Will disable popping jobs from the work queue.
				
				This is in order to avoid unnecessary unloading 
				of resources when swaping levels etc. 
				- stall loading.
				- call Unload for all previous content.
				- call GetResources( path, function ) for all the new content.
				- resume loading by calling RunLoading().
		*/
		void StallLoading();
		
		/*
			Will move all ingoing jobs to the work queue.
			See StallLoading for more info.
		*/
		void RunLoading();


	private:
		std::array< ResourceLoader*, sizeof...( LoadersDef ) >	loaders;	
		std::vector< Resource >									assetList;

		DefaultAllocator										contentAllocator;
	};

}

#endif


// TO DO: Add threadpool to the content manager
// TO DO: Add function for stalling/resuming the work queue in the threadpool
// TO DO: Add mutexes to respective position to secure the manager
// TO DO: Implement Load in ResourceLoader
// TO DO: Implement Unload in ResourceLoader
// TO DO: Implement insertSort of hashes to assetList
// TO DO: Implement binary search for hash in assetList 


