#include "ObjLoader.h"
//#include <fstream>
//#include "../shared/gfx/GFXInterface.hpp"
//#include "../shared/RenderInterface.h"
//
//extern RenderInterface* renderInterface;
namespace trr
{

	void* ObjLoader::internal_Load(DataContainer in)
	{
		if (in.data == nullptr)
			return nullptr;

		while (true)
		{
			char lineHeader[128];
			int res = scanf(in.data, "%s", lineHeader);

			if (res == EOF)
				break;

			if (strcmp(lineHeader, "v") == 0)
			{

			}
			else if (strcmp(lineHeader, "vt") == 0)
			{

			}
		}

		return nullptr;
	}

	void ObjLoader::internal_unload(void* data)
	{
		if (data)
		{
			//graphics unload
			m_pAllocator->deallocate(data);
			data = nullptr;
		}
	}
}