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

		virtual bool internal_Load(std::string path, Resource& r) = 0;
		virtual void internal_unload(Resource& r) = 0;

	public:
		virtual ~ResourceLoader();

		/*
			Loopthrough function for the asynchronous interface.
			Will check hash reference count before and after loading 
			in order to counter chained loading and unloading.
		*/
		bool Load(std::string path, Resource& r);

		/*
			Loopthrough function for the asynchronous interface.
			Will check hash reference count before and after loading 
			in order to counter chained loading and unloading.
		*/
		void Unload(Resource& r);

		/*
			Get the file extension associated with a particular
			resource loader. Used for matching a type of resource
			with a loader.
		*/
		virtual const std::string GetExtension() = 0;

	private:

	};

}


#endif