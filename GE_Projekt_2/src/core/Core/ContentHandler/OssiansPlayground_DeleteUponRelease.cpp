#include "OssiansPlayground_DeleteUponRelease.hpp"
#include <fstream>
#include <sstream>

namespace ossianTest
{
#pragma region Potato

	Potato::Potato() : root(nullptr), fileName("N/A")
	{

	}

	Potato::~Potato()
	{

	}

	void Potato::ReadHeader(const std::string& _packageName)
	{
		fileName = _packageName;

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
		}

		fs.close();
	}

	void* Potato::ReadAsset(const std::string &_fileName)
	{
		std::vector<std::string> path;
		std::stringstream ss(_fileName);
		std::string item;
		std::ifstream fs;

		while (std::getline(ss, item, '/')) 
		{
			path.push_back(item);
		}

		Asset* a = root->ReadAsset(path, 0);

		fs.open(fileName.c_str(), std::ifstream::binary);
		char* result = new char[a->GetSize()];
		fs.seekg(a->GetStart());
		fs.read(result, a->GetSize());

		return result;
	}

	Directory* Potato::ReadHeader(void* _data, uint& _readPointer)
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

	std::string Potato::ReadString(void* _data, uint& _readPointer)
	{
		char* data = (char*)_data;
		data = data + _readPointer;		
		uint size;
		memcpy(&size, data, sizeof(uint));
		std::string str(data + sizeof(uint), size);
		_readPointer += sizeof(uint) + size;

		return str;
	}

	uint Potato::ReadUint(void* _data, uint& _readPointer)
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

	Directory::Directory() : subDirectories(), assets()
	{
		name = "N/A";
	}

	Directory::Directory(std::string _name) : subDirectories(), assets()
	{
		name = _name;
	}

	Directory::~Directory()
	{

	}

	void Directory::AddSubDirectory(std::string _name, Directory* _directory)
	{
		subDirectories[_name] = _directory;
	}

	void Directory::AddAsset(std::string _name, Asset* _asset)
	{
		assets[_name] = _asset;
	}

	std::string Directory::GetName()
	{
		return name;
	}

	Asset* Directory::ReadAsset(const std::vector<std::string>& _path, uint _index)
	{
		if ((_path.size() - 1) == _index)
		{
			if (assets.find(_path[_index]) != assets.end())
			{
				return assets[_path[_index]];
			}

			return nullptr;
		}
		else if (subDirectories.find(_path[_index]) != subDirectories.end())
		{
			return subDirectories[_path[_index]]->ReadAsset(_path, _index + 1);
		}

		return nullptr;
	}

#pragma endregion

#pragma region Asset

	Asset::Asset() : name("N/A"), startByte(0), size(0)
	{
	}

	Asset::Asset(std::string _name, uint _startByte, uint _size) : name(_name), startByte(_startByte), size(_size)
	{
	}

	Asset::~Asset()
	{

	}

	void* Asset::GetData()
	{
		return nullptr;
	}

	std::string Asset::GetName()
	{
		return name;
	}

	uint Asset::GetSize()
	{
		return size;
	}

	uint Asset::GetStart()
	{
		return startByte;
	}

#pragma endregion
}