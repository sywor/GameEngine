#include "ObjLoader.h"
//#include <fstream>

#include "../shared/RenderInterface.h"
#include "../gfx/stdafx.h"
#include <Core/ContentHandler/Containers/ObjMesh.hpp>
extern std::mutex graphicMutex;

extern RenderInterface* renderInterface;
namespace trr
{
	
	void* ObjLoader::internal_Load(DataContainer in)
	{
		if (in.data == nullptr)
			return nullptr;

		std::stringstream stream(in.data);
		std::string line;
		std::vector<VECTOR4> vertices;
		std::vector<VECTOR2> uvs;
		std::vector<int> indices;
		std::vector<Vertex> vertexOut;

		int ind = 0;
		while (std::getline(stream, line))
		{
			std::string token;
			std::stringstream linestream(line);
			linestream >> token;

			if (token == "v")
			{
				float x, y, z;
				linestream >> x >> y >> z;
				vertices.push_back(VECTOR4(x, y, z, 1));

				indices.push_back(ind);
				ind++;
			}
			else if (token == "vt")
			{
				float u, v;
				linestream >> u >> v;
				uvs.push_back(VECTOR2(u, v));
			}
		}

		for (int i = 0; i < indices.size(); i++)
		{
			Vertex v;
			v.normal = VECTOR4(1, 1, 1, 1);
			v.pos = vertices[indices[i]];
			v.texC = uvs[indices[i]];
			vertexOut.push_back(v);
		}

		ObjMesh* mesh = (ObjMesh*)m_pAllocator->FlatAllocate(sizeof(ObjMesh));
		graphicMutex.lock();
		mesh->meshIndex = renderInterface->setMesh((void*)&vertexOut[0], vertexOut.size());
		graphicMutex.unlock();

		return mesh;
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