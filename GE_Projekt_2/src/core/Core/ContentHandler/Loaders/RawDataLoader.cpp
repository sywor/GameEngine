#include "RawDataLoader.hpp"
#include <fstream>
#include "../shared/gfx/GFXInterface.hpp"
namespace trr
{

	void* RawDataLoader::internal_Load( DataContainer in)
	{
		if (in.data == nullptr)
			return nullptr;

		BITMAPFILEHEADER* bmfh = (BITMAPFILEHEADER*)in.data;
		BITMAPINFOHEADER* bmih = (BITMAPINFOHEADER*)(in.data + sizeof(BITMAPFILEHEADER));
		FILE* f = fopen("testOut.bmp", "wb");
		//fwrite(&bmfh, 1, sizeof(BITMAPFILEHEADER), f);
		//fwrite(&bmih, 1, sizeof(BITMAPINFOHEADER), f);
		//fwrite(in.data, 1, bmih->biSizeImage, f);
		fwrite(in.data, 1, in.size, f);
		fclose(f);

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