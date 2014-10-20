#include "ResourceLoader.hpp"


namespace trr
{
	PoolAllocator *ResourceLoader::m_pAllocator;

	ResourceLoader::~ResourceLoader()
	{
	}


	void* ResourceLoader::Load( DataContainer _data)
	{
		// thread protection?
		return internal_Load( _data );
	}

		
	void ResourceLoader::Unload(Resource& r)
	{
		internal_unload( &r.data );
	}


}