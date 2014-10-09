#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "ResourceLoader.hpp"
#include "Resource.hpp"
#include "TemplateMagic.hpp"

#include <utility/asyncSettings.hpp>

#include "../Memory/DefaultAllocator.h"
#include <Core/ContentHandler/MurrMurr64.hpp>
#include <Core/ContentHandler/ZipHandler.hpp>
#include <Core/ContentHandler/DataContainer.hpp>
#include <Core/ContentHandler/CallbackContainer.hpp>
#include <Core/ThreadPool/Threadpool.hpp>
#include <cstdint>
#include <array>
#include <vector>
#include <string>
#include <functional>




#ifdef USE_ASYNC_LOCKING
#ifdef USE_CRITICAL_SECTION_LOCK
#include <Windows.h>
#define ENTER_CRITICAL_SECTION_ASSETLIST	EnterCriticalSection(&CriticalSection_AssetList)
#define EXIT_CRITICAL_SECTION_ASSETLIST		LeaveCriticalSection(&CriticalSection_AssetList)
#define ENTER_CRITICAL_SECTION_GENERAL		EnterCriticalSection(&CriticalSection_General)
#define EXIT_CRITICAL_SECTION_GENERAL		LeaveCriticalSection(&CriticalSection_General)
#define ENTER_CRITICAL_SECTION_GENERAL		EnterCriticalSection(&CriticalSection_zLib)
#define EXIT_CRITICAL_SECTION_GENERAL		LeaveCriticalSection(&CriticalSection_zLib)
#else
#include <mutex>
#define ENTER_CRITICAL_SECTION_ASSETLIST	mutex_assetList.lock()
#define EXIT_CRITICAL_SECTION_ASSETLIST		mutex_assetList.unlock()
#define ENTER_CRITICAL_SECTION_GENERAL		mutex_general.lock()
#define EXIT_CRITICAL_SECTION_GENERAL		mutex_general.unlock()
#define ENTER_CRITICAL_SECTION_ZLIB			mutex_zLib.lock()
#define EXIT_CRITICAL_SECTION_ZLIB			mutex_zLib.unlock()
#endif
#else
#define ENTER_CRITICAL_SECTION_ASSETLIST
#define EXIT_CRITICAL_SECTION_ASSETLIST
#define ENTER_CRITICAL_SECTION_GENERAL
#define EXIT_CRITICAL_SECTION_GENERAL
#define ENTER_CRITICAL_SECTION_ZLIB
#define EXIT_CRITICAL_SECTION_ZLIB
#endif


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
		std::uint64_t MakeHash( const void * key, int len )
		{
			return MurmurHash64(key, len, 42);
		}

		void OnAsyncLoadFinish( std::uint64_t hash, void* data )
		{
			for (int i = 0; i < callbackList.size(); i++)
			{
				if (callbackList[i].hash == hash)
				{
					workPool.Enqueue([hash, data, callbackList[i]]()
					{
						callbackList[i].callback( data );
					});

					callbackList.erase( callbackList.begin() + i );
					i--;
				}
			}
		}

	public: 

		// ///////////////////////////////////////////////////////////////////////////////// 
		// class utility
		ResourceManager()
			: contentAllocator(MemoryBlockSize, sizeOfMemory )
		{
#ifdef USE_CRITICAL_SECTION_LOCK
			if (!InitializeCriticalSectionAndSpinCount(&CriticalSection_AssetList, CRITICAL_SECTION_FAILED_INIT))
				return; // Should improve error handling / author
			if (!InitializeCriticalSectionAndSpinCount(&CriticalSection_General, CRITICAL_SECTION_FAILED_INIT))
				return; // Should improve error handling / author
			if (!InitializeCriticalSectionAndSpinCount(&CriticalSection_zLib, CRITICAL_SECTION_FAILED_INIT))
				return; // Should improve error handling / author
#endif

			workPool.Initialize( 2 ); 

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
		
		/*
		*/
		bool InitContentLib(const std::wstring contentLib)
		{
			ENTER_CRITICAL_SECTION_GENERAL;
			bool result = contentZipFile.Init(contentLib);
			EXIT_CRITICAL_SECTION_GENERAL;
			return result;
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
			std::uint64_t hash = MakeHash( path.data(), path.length() );
			Resource r;

			// return asset if already loaded
			ENTER_CRITICAL_SECTION_ASSETLIST;			
			if (assetList.find(hash) != assetList.end())
			{
				assetList[hash].nrReferences++;
				EXIT_CRITICAL_SECTION_ASSETLIST;
				return assetList[hash];
			}
			EXIT_CRITICAL_SECTION_ASSETLIST;
			
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

					ENTER_CRITICAL_SECTION_ZLIB;
					int zipId = contentZipFile.Find(fileName);
					bool zipReadResult = false;

					if (zipId != -1)
					{
						int fileLength	= contentZipFile.GetFileLen(zipId);
						rawData			= DataContainer(contentAllocator.allocate<char>(fileLength), fileLength);
						zipReadResult	= contentZipFile.ReadFile(zipId, rawData.data);
					}
					EXIT_CRITICAL_SECTION_ZLIB;

					if (!zipReadResult)
					{
						contentAllocator.deallocate(rawData.data);
					}
					else
					{
						if (loaders[i]->Load(fileName, r, rawData))
						{
							ENTER_CRITICAL_SECTION_ASSETLIST;
							assetList[hash] = r;
							EXIT_CRITICAL_SECTION_ASSETLIST;
						}
					}
					contentAllocator.deallocate(rawData.data);				
				}
			}
			return r;
		}

		/*
		Attempts to load asset and call the supplied function once loaded.
		data-pointer is pointer of the result container associated with the sough loader.
		*/
		void GetResource(std::string path, std::function<void(void* data)> callback)
		{
			// get hash
			std::uint64_t hash = MakeHash(path.data(), path.length());

			// return asset if already loaded
			ENTER_CRITICAL_SECTION_ASSETLIST;
			if (assetList.find(hash) != assetList.end())
			{
				assetList[hash].nrReferences++;
				EXIT_CRITICAL_SECTION_ASSETLIST;
				return assetList[hash];
			}
			EXIT_CRITICAL_SECTION_ASSETLIST;

			callbackList.push_back(CallbackContainer(hash, callback));

			ENTER_CRITICAL_SECTION_GENERAL;
			workPool.Enqueue( [ this, path, callback ]()
			{
				Resource res = GetResource( path );
				if ( /* resource is finished */ )
				OnAsyncLoadFinish( res.hash, res.data );
			});
			EXIT_CRITICAL_SECTION_GENERAL;
		}


		/*
			Will decrease the referense counter of the supplied hash
			and add job to unload should the number of referenses reach zero.
		*/
		void Unload(std::uint64_t handle)
		{
			Resource* toUnload = nullptr;

			ENTER_CRITICAL_SECTION_ASSETLIST;
			if (assetList.find(handle) != assetList.end())
			{
				Resource& r = assetList[handle];

				if (r.loaderIndex < loaders.size())	// find associated (un)loader --- this is a potential problem if we unmount loaders during runtime Resource should probably contain extension as well
				{
					if (--r.nrReferences <= 0)
					{
						toUnload = &r;
					}
				}
				else 
				{
					// the loaders have been tampered with!
					// we can't deallocate the data properly
				}
			}
			EXIT_CRITICAL_SECTION_ASSETLIST;

			if ( toUnload )
			{
				loaders[ toUnload->loaderIndex ]->Unload( *toUnload );
				
				ENTER_CRITICAL_SECTION_ASSETLIST;
				assetList.erase(handle);
				EXIT_CRITICAL_SECTION_ASSETLIST;
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
		std::map<std::uint64_t, Resource>	assetList;
		std::vector< CallbackContainer >	callbackList;

		PoolAllocator	contentAllocator;
		ZipFile			contentZipFile;
		ThreadPool		workPool;
		

#ifdef USE_CRITICAL_SECTION_LOCK
		CRITICAL_SECTION CriticalSection_AssetList;
		CRITICAL_SECTION CriticalSection_General;
		CRITICAL_SECTION CriticalSection_zLib;
#else
		std::mutex mutex_assetList;
		std::mutex mutex_general;
		std::mutex mutex_zLib;
#endif
	
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

// TO DO: erase of assetList in Unload must check if nrReferences is non-zero?
// TO DO: implement better init approach for threadpool nr threads and ev. other params.




