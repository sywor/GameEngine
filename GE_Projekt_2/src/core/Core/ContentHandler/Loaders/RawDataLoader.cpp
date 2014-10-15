#include "RawDataLoader.hpp"
#include <fstream>

namespace trr
{

	bool RawDataLoader::internal_Load(void* data, DataContainer _data)
	{
		if (_data.data == nullptr)
			return false;

		data = m_pAllocator->allocate<char>(_data.size);
		
		if (data == nullptr)
			return false;

		return true;
	}

	void RawDataLoader::internal_unload(void* data)
	{
		if (data)
		{
			m_pAllocator->deallocate(data);
			data = nullptr;
		}
	}
}