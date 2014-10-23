#include "Core\ContentHandler\Loaders\FreeImageLoader.hpp"

#include <../include/FreeImage/Dist/FreeImage.h>

#include <malloc.h>

void* FreeImageLoader::internal_Load(DataContainer in)
{
	int w = 100;
	int h = 100;
	int bpp = 4;
	FIBITMAP* bitmap = FreeImage_Allocate(w, h, bpp,0, 0, 0);
	memcpy(FreeImage_GetBits(bitmap), in.data, w * h * (bpp / 8));
	return bitmap;
}

void FreeImageLoader::internal_unload(void* data)
{
	FreeImage_Unload(static_cast<FIBITMAP*>(data));
}

