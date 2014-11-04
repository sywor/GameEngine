#include "RawDataLoader.hpp"
#include <fstream>
#include "../shared/gfx/GFXInterface.hpp"
#include "../shared/RenderInterface.h"

extern RenderInterface* renderInterface;
namespace trr
{

	void* RawDataLoader::internal_Load( DataContainer in)
	{
		if (in.data == nullptr)
			return nullptr;

		renderInterface->addTexture((uint8_t*)in.data, in.size);
		
		return nullptr;
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