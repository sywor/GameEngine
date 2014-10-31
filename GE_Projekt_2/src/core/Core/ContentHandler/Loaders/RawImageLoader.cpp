#include "Core\ContentHandler\Loaders\RawImageLoader.h"

#include <../include/FreeImage/Dist/FreeImage.h>
#include <fstream>
void* RawImageLoader::internal_Load(DataContainer in)
{
	if (in.data == nullptr)
		return nullptr;

	// because fuck you that's why!

	return nullptr;
}

void RawImageLoader::internal_unload(void* data)
{
	FreeImage_Unload(static_cast<FIBITMAP*>(data));
}

