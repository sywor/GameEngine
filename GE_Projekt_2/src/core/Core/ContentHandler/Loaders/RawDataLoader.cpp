#include "RawDataLoader.hpp"
#include <fstream>

namespace trr
{

	void* RawDataLoader::internal_Load( DataContainer in)
	{
		if (in.data == nullptr)
			return nullptr;

		void* out = m_pAllocator->allocate<char>(in.size);
		
		if (out == nullptr)
			return nullptr;

		return out;
	}

	void RawDataLoader::internal_unload( void* data )
	{
		if (data)
		{
			m_pAllocator->deallocate(data);
			data = nullptr;
		}
	}
}