#include "OssiansPlayground_DeleteUponRelease.hpp"
#include <fstream>
#include "utf8.hpp"

namespace ossianTest
{
#pragma region SorryForPotato

	SorryForPotato::SorryForPotato()
	{

	}

	SorryForPotato::~SorryForPotato()
	{

	}

	void SorryForPotato::ReadHeader(std::string _fileName)
	{
		fileName = _fileName;

		std::ifstream fs;
		fs.open(fileName.c_str(), std::ifstream::binary);

		if (fs.is_open())
		{
			char* buffer = new char[sizeof(uint)];
			fs.read(buffer, sizeof(uint));
			uint headerStart = *((uint*)buffer);
			fs.seekg(0, std::ifstream::end);
			uint fileEnd = fs.tellg();
			uint headerSize = fileEnd - headerStart;
			fs.seekg(headerStart);
			buffer = new char[headerSize];
			fs.read(buffer, headerSize);			

			uint readPointer = 0;
			root = ReadHeader(buffer, readPointer);

			int meh = 0;
		}

		fs.close();
	}

	Directory* SorryForPotato::ReadHeader(void* _data, uint& _readPointer)
	{
		std::string directoryName = ReadString(_data, _readPointer);
		uint directoryAssetCount = ReadUint(_data, _readPointer);

		Directory* dir = new Directory(directoryName);

		for (uint i = 0; i < directoryAssetCount; i++)
		{
			std::string name = ReadString(_data, _readPointer);
			uint start = ReadUint(_data, _readPointer);
			uint size = ReadUint(_data, _readPointer);
			Asset* a = new Asset(name, start, size);
			dir->AddAsset(a);
		}

		uint subDirectoryCount = ReadUint(_data, _readPointer);

		for (uint i = 0; i < subDirectoryCount; i++)
		{
			dir->AddSubDirectory(ReadHeader(_data, _readPointer));
		}

		return dir;
	}

	std::string SorryForPotato::ReadString(void* _data, uint& _readPointer)
	{
		char* data = (char*)_data;
		data = data + _readPointer;		
		uint size;
		memcpy(&size, data, sizeof(uint));
		std::string str(data + sizeof(uint), size);
		_readPointer += sizeof(uint) + size;

		return str;
	}

	uint SorryForPotato::ReadUint(void* _data, uint& _readPointer)
	{
		char* data = (char*)_data;
		data = data + _readPointer;
		uint result;
		memcpy(&result, data, sizeof(uint));
		_readPointer += sizeof(uint);

		return result;
	}

#pragma endregion

#pragma region Directory

	Directory::Directory(std::string _name)
	{
		name = _name;
	}

	Directory::~Directory()
	{

	}

	void Directory::AddSubDirectory(Directory* _directory)
	{
		subDirectories.push_back(_directory);
	}

	void Directory::AddAsset(Asset* _asset)
	{
		assets.push_back(_asset);
	}

	std::string Directory::GetName()
	{
		return name;
	}

#pragma endregion

#pragma region Asset

	Asset::Asset(std::string _name, uint _startByte, uint _size)
	{
		name = _name;
		startByte = _startByte;
		size = _size;
		data = nullptr;
	}

	Asset::~Asset()
	{

	}

	void* Asset::GetData()
	{
		return data;
	}

	std::string Asset::GetName()
	{
		return name;
	}

	uint Asset::GetSize()
	{
		return size;
	}

#pragma endregion
}