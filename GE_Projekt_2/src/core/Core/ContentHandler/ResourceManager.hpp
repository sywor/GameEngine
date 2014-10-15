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
			ResourceLoader::SetAllocator(&contentAllocator);
			InitializeArrayWithNew< LoadersDef... >( &loaders, &contentAllocator );
		}

		~ResourceManager()
		{
		}


		// ///////////////////////////////////////////////////////////////////////////////// 
		// content interface

		/*
			Attempts to load asset and returns void* to 
			respective result container associated with the sought loader.
		*/
		const Resource GetResource(std::string path)
		{
			// get hash
			std::uint64_t hash = MurmurHash64( path.data(), path.length(), 42);
			Resource r(hash);

			// return asset if already loaded
			if (assetList.find(hash) != assetList.end())
			{
				assetList[hash].nrReferences++;
				return assetList[hash];
			}
			
			// find compatible loader to new resource
			std::size_t sep = path.find_last_of(".");
			std::string fileName = path.substr(0, sep);
			const std::string ext = path.substr(sep+1);
			

			if (loaders.find(ext) != loaders.end())
			{
				int zipId = contentZipFile.Find(fileName);
				bool zipReadResult = false;
				DataContainer rawData;

				if (zipId != -1)
				{
					int fileLength = contentZipFile.GetFileLen(zipId);
					rawData = DataContainer(contentAllocator.allocate<char>(fileLength), fileLength);
					zipReadResult = contentZipFile.ReadFile(zipId, rawData.data);
				}

				if (zipReadResult && loaders[ext]->Load(r, rawData))
				{
					r.nrReferences++;
					r.loaderExtension = ext;
					assetList[hash] = r;
				}
				contentAllocator.deallocate(rawData.data);
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
			
			if (assetList.find(handle) != assetList.end())
			{
				Resource& r = assetList[handle];

				if (loaders.find(r.getExtension()) != loaders.end())
				{
					if (--r.nrReferences <= 0)
					{
						loaders[r.getExtension()]->Unload(r);
						assetList.erase(handle);
					}
				}
				else 
				{
					// the loaders have been tampered with!
					// we can't deallocate the data properly
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
		//std::array< ResourceLoader*, sizeof...( LoadersDef ) >	loaders;	
		//std::vector< Resource >									assetList;

		std::map<std::uint64_t, Resource>	assetList;
		std::map<const std::string, ResourceLoader*>	loaders;
		PoolAllocator						contentAllocator;
		ZipFile								contentZipFile;

		
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




