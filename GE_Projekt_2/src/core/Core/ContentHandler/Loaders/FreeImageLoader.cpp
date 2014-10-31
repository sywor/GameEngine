#include "Core\ContentHandler\Loaders\FreeImageLoader.hpp"

#include <../include/FreeImage/Dist/FreeImage.h>

#include <malloc.h>

void* FreeImageLoader::internal_Load(DataContainer in)
{
	int w = 675;
	int h = 647;
	int bpp = 24;
	FIBITMAP* bitmap = FreeImage_Allocate(w, h, bpp,0, 0, 0);
	if (!bitmap)
		return nullptr;

	BYTE* px = FreeImage_GetBits(bitmap);
	px[0] = 255;
	FreeImage_Save(FreeImage_GetFIFFromFilename("testImage.jpg"), bitmap, "testImage.jpg", 0);
	FreeImage_Unload(bitmap);

	FIBITMAP* fbm = FreeImage_ConvertFromRawBits((BYTE*)in.data, w, h, 12, bpp, 0, 0, 0, 0);
	FreeImage_Save(FreeImage_GetFIFFromFilename("testImageA.jpg"), bitmap, "testImageA.jpg", 0);
	FreeImage_Unload(fbm);

	return bitmap;
}

void FreeImageLoader::internal_unload(void* data)
{
	FreeImage_Unload(static_cast<FIBITMAP*>(data));
}

