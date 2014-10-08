#include "RawDataLoader.hpp"
#include <fstream>

namespace trr
{

	bool RawDataLoader::internal_Load(std::string path, Resource& r)
	{
		std::fstream stream(path, std::ios::in);

		// file not found
		if (!stream)
		{
			return false;
		}
		
		// get length of file
		stream.seekg(0, std::ios::end);
		std::streamsize size = stream.tellg();
		stream.seekg(0, std::ios::beg);

		// this is not allocated properly!
		r.data = new char[size+1];
		((char*)r.data)[size] = 0;

		// unable to read file to buffer
		if (!stream.read(static_cast<char*>(r.data), size))
		{
			return false;
		}

		stream.close();

		return true;
	}

	void RawDataLoader::internal_unload(Resource& r)
	{
		if (r.data)
		{
			printf("%s\n", (char*)r.data);
			delete [] r.data;
			r.data = nullptr;
		}
	}
}