#ifndef RESOURCE_LOADER_H
#define RESOURCE_LOADER_H

#include "Resource.hpp"

#include <string>
#include <cstdint>

namespace trr
{
	
	class ResourceLoader abstract
	{
	private:

		virtual void internal_Load( std::string path, std::uint64_t hash )	= 0;
		virtual void internal_unload( std::uint64_t hash )					= 0;

	public:
		virtual ~ResourceLoader();

		/*
			Loopthrough function for the asynchronous interface.
			Will check hash reference count before and after loading 
			in order to counter chained loading and unloading.
		*/
		void Load( std::string path, std::uint64_t hash );

		/*
			Loopthrough function for the asynchronous interface.
			Will check hash reference count before and after loading 
			in order to counter chained loading and unloading.
		*/
		void Unload( std::uint64_t hash );


	private:

	};

}


#endif