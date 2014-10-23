#include <fstream>
#include <sstream>
#include "Gun.hpp"

namespace Potato
{

	Gun::Gun(PoolAllocator* _allocator) : root(nullptr), fileName("N/A"), allocator(_allocator)
	{

	}

	Gun::~Gun()
	{

	}

	void Gun::Load(const std::string& _spudName)
	{
		fileName = _spudName;

		std::ifstream fs;
		fs.open(fileName.c_str(), std::ifstream::binary);

		if (fs.is_open())
		{			
			uint headerStart;
			fs.read((char*)&headerStart, sizeof(uint));
			fs.seekg(0, std::ifstream::end);
			uint fileEnd = fs.tellg();
			uint headerSize = fileEnd - headerStart;
			fs.seekg(headerStart);
			char* buffer = allocator->allocate<char>(headerSize);
			fs.read(buffer, headerSize);

			uint readPointer = 0;
			root = ReadHeader(buffer, readPointer);

			allocator->deallocate(buffer);
		}

		fs.close();
	}

	void* Gun::Shoot(const std::string& _spudName)
	{
		std::vector<std::string> path;
		std::stringstream ss(_spudName);
		std::string item;
		std::ifstream fs;

		while (std::getline(ss, item, '/'))
		{
			path.push_back(item);
		}

		Asset* a = root->ReadAsset(path, 0);

		fs.open(fileName.c_str(), std::ifstream::binary);
		char* result = allocator->allocate<char>(a->GetSize());
		fs.seekg(a->GetStart());
		fs.read(result, a->GetSize());

		return result;
	}

	Directory* Gun::ReadHeader(void* _data, uint& _readPointer)
	{
		std::string directoryName = ReadString(_data, _readPointer);
		uint directoryAssetCount = ReadUint(_data, _readPointer);

		Directory* dir = allocator->allocate<Directory>(directoryName);

		for (uint i = 0; i < directoryAssetCount; i++)
		{
			std::string name = ReadString(_data, _readPointer);
			uint start = ReadUint(_data, _readPointer);
			uint size = ReadUint(_data, _readPointer);
			Asset* a = allocator->allocate<Asset>(name, start, size);
			dir->AddAsset(name, a);
		}

		uint subDirectoryCount = ReadUint(_data, _readPointer);

		for (uint i = 0; i < subDirectoryCount; i++)
		{
			Directory* tmp = ReadHeader(_data, _readPointer);
			dir->AddSubDirectory(tmp->GetName(), tmp);
		}

		return dir;
	}

	std::string Gun::ReadString(void* _data, uint& _readPointer)
	{
		char* data = (char*)_data;
		data = data + _readPointer;
		uint size;
		memcpy(&size, data, sizeof(uint));
		std::string str(data + sizeof(uint), size);
		_readPointer += sizeof(uint)+size;

		return str;
	}

	uint Gun::ReadUint(void* _data, uint& _readPointer)
	{
		char* data = (char*)_data;
		data = data + _readPointer;
		uint result;
		memcpy(&result, data, sizeof(uint));
		_readPointer += sizeof(uint);

		return result;
	}
}