#include "RawDataLoader.hpp"
#include <fstream>

namespace trr
{

	bool RawDataLoader::internal_Load(std::string path, Resource& r, DataContainer _data)
	{
		if (_data.data == nullptr)
			return false;

		r.data = m_pAllocator->allocate<char>(_data.size);
		
		if (r.data == nullptr)
			return false;

		return true;
	}

	void RawDataLoader::internal_unload(Resource& r)
	{
		if (r.data)
		{
			m_pAllocator->deallocate(r.data);
			r.data = nullptr;
		}
	}
}