#include "RawDataLoader.hpp"
#include <fstream>

namespace trr
{

	bool RawDataLoader::internal_Load(void** out, DataContainer in)
	{
		if (in.data == nullptr)
			return false;

		*out = m_pAllocator->allocate<char>(in.size);
		
		if (*out == nullptr)
			return false;

		return true;
	}

	void RawDataLoader::internal_unload(void** data)
	{
		if (*data)
		{
			m_pAllocator->deallocate(data);
			data = nullptr;
		}
	}
}