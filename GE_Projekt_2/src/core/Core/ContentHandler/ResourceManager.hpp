#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "ResourceLoader.hpp"
#include "Resource.hpp"
#include "TemplateMagic.hpp"

#include "../Memory/DefaultAllocator.h"
#include <Core/ContentHandler/MurrMurr64.hpp>
#include <Core/ContentHandler/ZipHandler.hpp>
#include <Core/ContentHandler/DataContainer.hpp>
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

	template< int MemoryBlockSize, int sizeOfMemory, typename... LoadersDef >
	class ResourceManager
	{
	private:

	public: 

		// ///////////////////////////////////////////////////////////////////////////////// 
		// class utility
		ResourceManager()
			: contentAllocator(MemoryBlockSize, sizeOfMemory )
		{
			for( int i = 0; i < sizeof...(LoadersDef); i++ )
			{
				loaders[ i ] = nullptr;
			}

			ResourceLoader::SetAllocator(&contentAllocator);
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
		const Resource GetResource(std::string path)
		{
			// implement proper hash!!!
			std::uint64_t hash = MurmurHash64( path.data(), path.length(), 42);
			Resource r;

			// return asset if already loaded
			for (Resource& asset : assetList)
			{
				if (asset.hash == hash)
				{
					asset.nrReferences++;
					return asset;
				}
			}
			
			// find compatible loader to new resource
			std::size_t sep = path.find_last_of(".");
			std::string fileName = path.substr(0, sep);
			const std::string ext = path.substr(sep+1);
			for (int i = 0; i < loaders.size(); ++i)
			{
				if (MatchExtension(ext.c_str(), loaders[i]->GetExtension().c_str()))
				{
					
					r.hash = hash;
					r.nrReferences++;
					r.loaderIndex = i;

					DataContainer rawData;
					int zipId = contentZipFile.Find(fileName);
					bool zipReadResult = false;

					if (zipId != -1)
					{
						int fileLength	= contentZipFile.GetFileLen(zipId);
						rawData = DataContainer(contentAllocator.allocate<char>(fileLength), fileLength);
						zipReadResult	= contentZipFile.ReadFile(zipId, rawData.data);
					}

					if (!zipReadResult)
					{
						contentAllocator.deallocate(rawData.data);
					}
					else
					{
						if (loaders[i]->Load(fileName, r, rawData))
						{
							assetList.push_back(r);
						}
					}
					contentAllocator.deallocate(rawData.data);				
				}
			}
			return r;
		}

		/*
		*/
		bool InitContentLib(const std::wstring contentLib)
		{
			if (!contentZipFile.Init(contentLib))
				return false;

			return true;
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
			for (int i = 0; i < assetList.size(); ++i)
			{
				if (assetList[i].hash == handle)
				{
					if (assetList[i].loaderIndex < loaders.size())	// find associated unloader
					{
						assetList[i].nrReferences--;

						// unload
						if (assetList[i].nrReferences <= 0)
						{
							loaders[assetList[i].loaderIndex]->Unload(assetList[i]);
							assetList.erase(assetList.begin() + i);
							break;
						}
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

		PoolAllocator	contentAllocator;
		ZipFile			contentZipFile;
	};

}

#endif


// TO DO: Add threadpool to the content manager
// TO DO: Add function for stalling/resuming the work queue in the threadpool
// TO DO: Add mutexes to respective position to secure the manager
// TO DO: Implement insertSort of hashes to assetList
// TO DO: Implement binary search for hash in assetList 
// TO DO: Fix so path is check outside of zlib if path is not found in the zlib table. 
//		  [ Under discussion and if time is available ]




