#include "ResourceLoader.hpp"



namespace trr
{
	
	ResourceLoader::~ResourceLoader()
	{
	}


	bool ResourceLoader::Load(std::string path, Resource& r)
	{
		// thread protection?
		return internal_Load(path, r);
	}

		
	void ResourceLoader::Unload(Resource& r)
	{
		// thread protection?
		if (--r.nrReferences == 0)
			internal_unload(r);
	}


}