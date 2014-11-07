#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "ResourceLoader.hpp"
#include "Resource.hpp"
#include "TemplateMagic.hpp"

#include <utility/asyncSettings.hpp>

#include "../Memory/DefaultAllocator.h"
#include <Core/ContentHandler/MurrMurr64.hpp>
#include <Core/ContentHandler/ContainerLoaders/ZipReader.hpp>
#include <Core/ContentHandler/ContainerLoaders/PotatoGun.hpp>
#include <Core/ContentHandler/ContainerLoaders/PackageHandler.hpp>
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
#define CONTENT_CHECK_VALID_DATA( dataPointer )		( dataPointer != nullptr \
													&& dataPointer != CONTENT_CALLBACK_CAN_NOT_FIND_FILE \
													&& dataPointer != CONTENT_CALLBACK_LOADING_FAILED \
													&& dataPointer != CONTENT_CALLBACK_NO_LOADER_ACCEPTS_FILE \
													&& dataPointer != CONTENT_CALLBACK_OUT_OF_MEMORY )

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
#define ENTER_CRITICAL_SECTION_PACKAGES		mutex_packages.lock()
#define EXIT_CRITICAL_SECTION_PACKAGES		mutex_packages.unlock()
#endif
#else
#define ENTER_CRITICAL_SECTION_ASSETLIST
#define EXIT_CRITICAL_SECTION_ASSETLIST
#define ENTER_CRITICAL_SECTION_GENERAL
#define EXIT_CRITICAL_SECTION_GENERAL
#define ENTER_CRITICAL_SECTION_PACKAGES
#define EXIT_CRITICAL_SECTION_PACKAGES
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

	template< int memoryBlockSize, int sizeOfMemory, int internalMetaMemoryLimit, typename... LoadersDef >
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
				ENTER_CRITICAL_SECTION_PACKAGES;
				DataContainer rawData = packagehandler->ReadAsset(fileName);
				EXIT_CRITICAL_SECTION_PACKAGES;

				if (rawData.err == PackageResult::PACKAGE_OK)
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

			ENTER_CRITICAL_SECTION_ASSETLIST;
			Resource assetRef = assetList[hash];
			if( assetRef.path == nullptr && CONTENT_CHECK_VALID_DATA( data ) )
			{
				char* pathMemory = (char*)contentAllocator.FlatAllocate( path.size() + 1 );
				if( pathMemory != nullptr )
				{
					std::memcpy( pathMemory, path.c_str(), path.length() );
					char tempChar = '\0';
					std::memcpy( pathMemory + path.size(), &tempChar, 1 );
					assetList[ hash ].path = pathMemory;
					assetRef.path = pathMemory;
				}
				else
				{
					// handle error
					contentAllocator.deallocate( data );
					data = CONTENT_CALLBACK_OUT_OF_MEMORY;
				}
			}

			if( assetRef.loaderExtension == nullptr && assetRef.path != nullptr && CONTENT_CHECK_VALID_DATA( data ) )
			{
				char* extMemory = (char*)contentAllocator.FlatAllocate( ext.size() + 1 );
				if( extMemory != nullptr )
				{
					std::memcpy( extMemory, ext.data(), ext.size() );
					char tempChar = '\0';
					std::memcpy( extMemory + ext.size(), &tempChar, 1 );
					assetList[ hash ].loaderExtension = extMemory;
				}
				else
				{
					// handle error
					contentAllocator.deallocate( data );
					data = CONTENT_CALLBACK_OUT_OF_MEMORY;
				}
			}

			if( CONTENT_CHECK_VALID_DATA( data ) )
			{
				// resource existance is guaranteed by caller
				Resource assetRef = assetList[hash];
				assetRef.state	= (assetRef.nrReferences == 0) ? assetRef.state : RState::READY;
				assetRef.data	= data;
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
			}
			else
			{
				// error occurred
				// remove asset instance
				// run callbacks with tagged parameters
				assetList.erase( hash );
				RunCallbacks( hash, data );
			}
			EXIT_CRITICAL_SECTION_ASSETLIST;
		}

		void RunCallbacks( std::uint64_t hash, void* data )
		{
			for( int i = 0; i < callbackList.size(); i++ )
			{
				if( callbackList[ i ].hash == hash )
				{
					std::function< void(void*, std::uint64_t hash) > func = callbackList[i].callback;
					workPool.Enqueue( [ func, data, hash ]()
					{
						func(data, hash);
					});
					callbackList.erase( callbackList.begin() + i );
					i--;
				}
			}
		}

		void VolatileSetAssetState( std::uint64_t hash, RState state )
		{
			assetList[ hash ].state = state;
		}

		void VolatileDeallocatePathAndExtension( std::uint64_t hash )
		{
			if( assetList[ hash ].loaderExtension )
				contentAllocator.deallocate( assetList[ hash ].loaderExtension );
			if( assetList[ hash ].path )
				contentAllocator.deallocate( assetList[ hash ].path );
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
						VolatileSetAssetState( hash, RState::LOADING );
						std::string path = assetList[ hash ].getPath();
						workPool.Enqueue( [ this, hash, path ]()
						{
							LoadResource( path );
						});
					}
					else
					{
						VolatileDeallocatePathAndExtension( hash );
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
			: contentAllocator(memoryBlockSize, sizeOfMemory), packagehandler(contentAllocator.allocate<packageHandler>())
		{

			#ifdef USE_CRITICAL_SECTION_LOCK
			if (!InitializeCriticalSectionAndSpinCount(&CriticalSection_AssetList, CRITICAL_SECTION_FAILED_INIT))
				return; // Should improve error handling / author
			if (!InitializeCriticalSectionAndSpinCount(&CriticalSection_General, CRITICAL_SECTION_FAILED_INIT))
				return; // Should improve error handling / author
			if (!InitializeCriticalSectionAndSpinCount(&CriticalSection_zLib, CRITICAL_SECTION_FAILED_INIT))
				return; // Should improve error handling / author
			#endif

			// calculate memory limits for metadata
			// assume general purpose configuration: 50/50 split 
			memoryLimit = internalMetaMemoryLimit;
			int split = internalMetaMemoryLimit / 2;
			callbackListSizeLimit	= split / sizeof( CallbackContainer );
			assetListSizeLimit		= split / sizeof( std::pair< std::uint64_t, Resource > );

			inOutStalled = false;
			workPool.Initialize( 2 );
			ResourceLoader::SetAllocator(&contentAllocator);
			InitializeArrayWithNew< LoadersDef... >( &loaders, &contentAllocator );
		}

		~ResourceManager()
		{
			workPool.KillThreads();
		}

		void InitContentLibs(std::initializer_list<std::string> _packages)
		{
			for each (std::string str in _packages)
			{
				int dotIndex = str.find_last_of('.');
				std::string extention = str.substr(dotIndex + 1);
				PackageResult pr;
				if (extention == "Spud")
				{
					PotatoGun* pg = contentAllocator.allocate<PotatoGun>(&contentAllocator);
					pr = pg->LoadPackage(str);

					if (pr != PackageResult::PACKAGE_OK)
					{
						LOG_ERROR << PackageResultToString(pr) << std::endl;
						continue;
					}

					packagehandler->AddHandle(pg);
				}
				else if (extention == "zip")
				{
					ZipReader* zr = contentAllocator.allocate<ZipReader>(&contentAllocator);
					pr = zr->LoadPackage(str);

					if (pr != PackageResult::PACKAGE_OK)
					{
						LOG_ERROR << PackageResultToString(pr) << std::endl;
						continue;
					}

					packagehandler->AddHandle(zr);
				}
			}
		}

		void DumpAssetList()
		{
			ENTER_CRITICAL_SECTION_ASSETLIST;
			for( auto res = assetList.begin(); res != assetList.end(); res++ )
			{
				LOG_DEBUG <<  "asset dump - nrRefs: " << res->second.getReferences() << "  " << res->second.getPath() << std::endl;
			}
			EXIT_CRITICAL_SECTION_ASSETLIST;
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
		void GetResource(std::string path, std::function<void(const void* data, std::uint64_t hash)> callback)
		{
			std::uint64_t hash = MakeHash( path.data(), path.size() );

			ENTER_CRITICAL_SECTION_ASSETLIST;
			if( callbackList.size() >= callbackListSizeLimit )
			{
				callback( CONTENT_CALLBACK_OUT_OF_MEMORY, hash );
				EXIT_CRITICAL_SECTION_ASSETLIST;
				return;
			}
			else if( assetList.find( hash ) == assetList.end() )
			{
				// if asset is not existing, add it to the queue
				if( assetList.size() < assetListSizeLimit )
				{
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
						char* p = (char*)contentAllocator.FlatAllocate( path.size() + 1 );
						if( p != nullptr )
						{
							std::memcpy( p, path.data(), path.size() );
							char tempChar = '\0';
							std::memcpy( p + path.size(), &tempChar, 1 );
							assetList[ hash ].path	= p;
						}
						else
						{
							// handle error
							RunCallbacks( hash, CONTENT_CALLBACK_OUT_OF_MEMORY );
							assetList.erase( hash );
							EXIT_CRITICAL_SECTION_ASSETLIST;
							return;
						}
					}
				}
				else
				{
					callback( CONTENT_CALLBACK_OUT_OF_MEMORY, hash );
					EXIT_CRITICAL_SECTION_ASSETLIST;
					return;
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
				workPool.Enqueue( [ callback, data, hash ]()
				{
					callback( data, hash );
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
						std::string extension = assetList[ hash ].getExtension();
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
						std::string extension = assetList[ res->second.hash ].getExtension();					
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
		packageHandler* packagehandler;
		ThreadPool		workPool;

		bool			inOutStalled;
		
		int				memoryLimit;
		int				callbackListSizeLimit;
		int				assetListSizeLimit;

#ifdef USE_CRITICAL_SECTION_LOCK
		CRITICAL_SECTION CriticalSection_AssetList;
		CRITICAL_SECTION CriticalSection_General;
		CRITICAL_SECTION CriticalSection_zLib;
#else
		std::mutex mutex_assetList;
		std::mutex mutex_general;
		std::mutex mutex_packages;
#endif
	};
}

#endif


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