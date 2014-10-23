#include "Core\ContentHandler\Loaders\RawImageLoader.h"

#include <../include/FreeImage/Dist/FreeImage.h>

void* RawImageLoader::internal_Load(DataContainer in)
{
	if (in.data == nullptr)
		return nullptr;

	// out = alloc(sizeof(srvPtr)
	// out = DXfromMemory
	void* out = m_pAllocator->allocate<char>(in.size);

	if (out == nullptr)
		return nullptr;

	return out;
}

void RawImageLoader::internal_unload(void* data)
{
	FreeImage_Unload(static_cast<FIBITMAP*>(data));
}

