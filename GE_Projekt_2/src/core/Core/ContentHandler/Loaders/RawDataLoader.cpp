#include "RawDataLoader.hpp"
#include <fstream>
#include "../shared/RenderInterface.h"
#include <Core/ContentHandler/Containers/TextureContainer.hpp>

extern RenderInterface* renderInterface;
extern std::mutex graphicMutex;

namespace trr
{

	void* RawDataLoader::internal_Load( DataContainer in)
	{
		graphicMutex.lock();
		if (in.data == nullptr)
		{
			graphicMutex.unlock();
			return nullptr;
		}

		TextureContainer* texture = (TextureContainer*)m_pAllocator->FlatAllocate(sizeof(TextureContainer));

		if (texture != nullptr)
		{
			texture->textureIndex = renderInterface->addTexture((uint8_t*)in.data, in.size);
		}
		
		graphicMutex.unlock();
		return texture;
	}

	void RawDataLoader::internal_unload( void* data )
	{
		if (data)
		{
			//graphics unload
			m_pAllocator->deallocate(data);
			data = nullptr;
		}
	}
}