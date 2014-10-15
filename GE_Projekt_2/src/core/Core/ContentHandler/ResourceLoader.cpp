#include "ResourceLoader.hpp"


namespace trr
{
	PoolAllocator *ResourceLoader::m_pAllocator;

	ResourceLoader::~ResourceLoader()
	{
	}


	bool ResourceLoader::Load(Resource& r, DataContainer _data)
	{
		// thread protection?
		return internal_Load( &r.data, _data );
	}

		
	void ResourceLoader::Unload(Resource& r)
	{
		internal_unload( &r.data );
	}


}