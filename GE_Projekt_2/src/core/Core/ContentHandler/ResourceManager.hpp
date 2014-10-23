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
#include <logger/Logger.hpp>
#include <cstdint>
#include <array>
#include <vector>
#include <string>
#include <functional>

#define CONTENT_CALLBACK_CAN_NOT_FIND_FILE			(void*)1
#define CONTENT_CALLBACK_NO_LOADER_ACCEPTS_FILE		(void*)2
#define CONTENT_CALLBACK_LOADING_FAILED				(void*)3
#define CONTENT_CALLBACK_OUT_OF_MEMORY				(void*)4
#define CONTENT_CALLBACK_INVALID_COMPRESSION		(void*)5
#define CONTENT_CHECK_VALID_DATA( dataPointer )		( dataPointer != nullptr \
													&& dataPointer != CONTENT_CALLBACK_CAN_NOT_FIND_FILE \
													&& dataPointer != CONTENT_CALLBACK_LOADING_FAILED \
													&& dataPointer != CONTENT_CALLBACK_NO_LOADER_ACCEPTS_FILE \
													&& dataPointer != CONTENT_CALLBACK_OUT_OF_MEMORY \
													&& datapointer != CONTENT_CALLBACK_INVALID_COMPRESSION )

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
		
		void LoadResource( std::string path )
		{
			std::uint64_t hash = MakeHash( path.data(), path.length() );

			// find compatible loader to new resource
			std::size_t sep = path.find_last_of(".");
			std::string fileName = path.substr(0, sep);
			const std::string ext = path.substr(sep + 1);

			void* data = nullptr;
			if (loaders.find(ext) != loaders.end())
			{
				ENTER_CRITICAL_SECTION_ZLIB;
				int zipId = contentZipFile.Find(fileName);
				bool zipReadResult = false;
				DataContainer rawData;

				if (zipId != -1)
				{
					int fileLength = contentZipFile.GetFileLen(zipId);
					rawData = DataContainer(contentAllocator.allocate<char>(fileLength), fileLength);
					if( rawData.data == nullptr )
					{
						data = CONTENT_CALLBACK_OUT_OF_MEMORY;
					}
					else
					{
						zipReadResult = contentZipFile.ReadFile(zipId, rawData.data);
					}
				}
				EXIT_CRITICAL_SECTION_ZLIB;

				if( zipReadResult )
				{
					if( (data = loaders[ext]->Load( rawData )) == nullptr )
					{
						data = CONTENT_CALLBACK_CAN_NOT_FIND_FILE;
					}
				}
				else if( data != CONTENT_CALLBACK_OUT_OF_MEMORY )
				{
					data = CONTENT_CALLBACK_CAN_NOT_FIND_FILE;
				}
				contentAllocator.deallocate(rawData.data);
			}
			else
			{
				data = CONTENT_CALLBACK_NO_LOADER_ACCEPTS_FILE;
			}
			if( CONTENT_CHECK_VALID_DATA( data ) )
			{
				// resource existance is guaranteed by caller
				ENTER_CRITICAL_SECTION_ASSETLIST;
				Resource assetRef = assetList[hash];
				assetRef.path	= path;
				assetRef.state	= (assetRef.nrReferences == 0) ? assetRef.state : RState::READY;
				assetRef.data	= data;
				assetRef.loaderExtension = ext;
				assetList[hash]	= assetRef;

				// always run callbacks
				RunCallbacks( hash, data );

				// check if unloaded while loading
				if( assetRef.nrReferences == 0 )
				{
					std::string extension = assetList[ hash ].getExtension();
					workPool.Enqueue( [ this, hash, extension ]()
					{
						UnloadResource( hash, extension );
					});
				}
				EXIT_CRITICAL_SECTION_ASSETLIST;
			}
			else
			{
				// error occurred
				// remove asset instance
				// run callbacks with tagged parameters
				ENTER_CRITICAL_SECTION_ASSETLIST;
				assetList.erase( hash );
				RunCallbacks( hash, data );
				EXIT_CRITICAL_SECTION_ASSETLIST;
			}
		}

		void RunCallbacks( std::uint64_t hash, void* data )
		{
			for( int i = 0; i < callbackList.size(); i++ )
			{
				if( callbackList[ i ].hash == hash )
				{
					std::function< void( void* ) > func = callbackList[ i ].callback;
					workPool.Enqueue( [ func, data ]()
					{
						func( data );
					});
					callbackList.erase( callbackList.begin() + i );
					i--;
				}
			}
		}

		void VolotileSetAssetState( std::uint64_t hash, RState state )
		{
			assetList[ hash ].state = state;
		}

		void UnloadResource( std::uint64_t hash, std::string extension )
		{
			if( loaders.find( extension ) != loaders.end() )
			{						
				workPool.Enqueue( [ this, extension, hash ]()
				{
					loaders[ extension ]->Unload( (void*)assetList[ hash ].getData() );
					ENTER_CRITICAL_SECTION_ASSETLIST;
					// the resource has been loaded while this function was loading
					// add load call to queue
					unsigned int nrRefs = assetList[hash].getReferences();
					if( nrRefs > 0 )
					{
						VolotileSetAssetState( hash, RState::LOADING );
						std::string path = assetList[ hash ].getPath();
						workPool.Enqueue( [ this, hash, path ]()
						{
							LoadResource( path );
						});
					}
					else
					{
						assetList.erase( hash );
					}

					EXIT_CRITICAL_SECTION_ASSETLIST;
				});
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

			inOutStalled = false;
			workPool.Initialize( 2 );
			ResourceLoader::SetAllocator(&contentAllocator);
			InitializeArrayWithNew< LoadersDef... >( &loaders, &contentAllocator );
			contentZipFile = ZipFile(contentAllocator);
		}

		~ResourceManager()
		{
			workPool.KillThreads();
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
			Blocking function but still uses threading in the background.
			Attempts to load asset and returns void* to 
			respective result container associated with the sought loader.
		*/
		const Resource GetResource( std::string path )
		{
			volatile int flag = 0;
			void* errorMessage = nullptr;
			std::uint64_t hash = MakeHash( path.data(), path.size() );
			Resource res;
			
			GetResource( path, [ &flag, &errorMessage ]( const void* data )
			{
				errorMessage = (void*)data;
				flag = 1;
			});

			while( flag == 0 );
			ENTER_CRITICAL_SECTION_ASSETLIST;
			if( assetList.find( hash ) != assetList.end() )
			{
				res = assetList[ hash ];
			}
			else
			{
				res.data = errorMessage;
			}
			EXIT_CRITICAL_SECTION_ASSETLIST;
			return res;
		}

		/*
			Attempts to load asset and call the supplied function once loaded.
			data-pointer is pointer of the result container associated with the sough loader.
		*/
		void GetResource(std::string path, std::function<void(const void* data)> callback)
		{
			std::uint64_t hash = MakeHash( path.data(), path.size() );
			
			ENTER_CRITICAL_SECTION_ASSETLIST;
			if( assetList.find( hash ) == assetList.end() )
			{
				// if asset is not existing, add it to the queue
				Resource res( hash );
				assetList[ hash ] = res;
				callbackList.push_back( CallbackContainer( hash, callback ));
				if( !inOutStalled )
				{
					workPool.Enqueue( [ this, path ]()
					{
						LoadResource( path );
					});
				}
				else
				{
					assetList[ hash ].state = RState::WAITING_LOAD;
					assetList[ hash ].path	= path;
				}
			}
			else if( assetList[ hash ].state != RState::READY )
			{
				// add callback to list
				callbackList.push_back( CallbackContainer( hash, callback ));
			}
			else
			{
				// add callback to queue
				void* data = assetList[ hash ].data;
				workPool.Enqueue( [ callback, data ]()
				{
					callback( data );
				});
			}
			assetList[ hash ].nrReferences++;
			EXIT_CRITICAL_SECTION_ASSETLIST;
		}


		/*
			Loop thorugh function for Unload( std::uint64_t ) using path
			as parameter.
		*/
		void Unload(std::string path )
		{
			std::uint64_t hash = MakeHash(path.data(), path.length());
			Unload( hash );
		}

		/*
			Will decrease the referense counter of the supplied hash
			and add job to unload should the number of referenses reach zero.
		*/
		void Unload( std::uint64_t hash )
		{
			ENTER_CRITICAL_SECTION_ASSETLIST;
			if( assetList.find( hash ) != assetList.end() )
			{
				assetList[ hash ].nrReferences--;
				if( assetList[ hash ].nrReferences == 0 && assetList[ hash ].state == RState::READY )
				{
					if( !inOutStalled )
					{
						assetList[ hash ].state = RState::UNLOADING;
						std::string extension = assetList[ hash ].loaderExtension;					
						workPool.Enqueue( [ this, hash, extension ]()
						{
							UnloadResource( hash, extension );
						});
					}
				}
			}
			EXIT_CRITICAL_SECTION_ASSETLIST;
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
		void StallLoading()
		{
			ENTER_CRITICAL_SECTION_GENERAL;
			inOutStalled = true;
			EXIT_CRITICAL_SECTION_GENERAL;
		}
		
		/*
			Will move all ingoing jobs to the work queue.
			See StallLoading for more info.
		*/
		void RunLoading()
		{
			ENTER_CRITICAL_SECTION_ASSETLIST;
			inOutStalled = true;
			for( auto res = assetList.begin(); res != assetList.end(); res++ )
			{
				if( res->second.nrReferences == 0 )
				{
					if( res->second.state == RState::READY )
					{
						std::string path = res->second.getPath();
						assetList[ res->second.hash ].state = RState::UNLOADING;
						std::string extension = assetList[ res->second.hash ].loaderExtension;					
						std::uint64_t hash = res->second.hash;
						workPool.Enqueue( [ this, hash, extension, path ]()
						{
							UnloadResource( hash, extension );
						});
					}
				}
				else if( res->second.state == RState::WAITING_LOAD )
				{
					std::string path = res->second.getPath();
					workPool.Enqueue( [ this, path ]()
					{
						LoadResource( path );
					});
				}
			}
			EXIT_CRITICAL_SECTION_ASSETLIST;
		}


	private:
		std::vector< CallbackContainer > callbackList;
		std::map< std::uint64_t, Resource >	assetList;
		std::map< const std::string, ResourceLoader* >	loaders;
		

		PoolAllocator	contentAllocator;
		ZipFile			contentZipFile;
		ThreadPool		workPool;

		bool			inOutStalled;
		

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


// TO DO: Fix so path is check outside of zlib if path is not found in the zlib table. 
//		  [ Under discussion and if time is available ]


/* labb 2 requirements

	- thread safe.
	- hard limit memory usage.
	- unload assets to free memory.
	- dump list of currently loaded memory.
	- test loaders
	- guid
	- stress scenario

*/

/* labb 3 
	
	e - graphical representation
	e - use memory allocators
	e - trivial report
		d - 2 package formats, 2 asset formats
			c - implement and explain 2 features not required by this or the 
			    the previous assignemnts. may already be in use.
					b - offline tool
						a - formal report / stopid paper
				


*/