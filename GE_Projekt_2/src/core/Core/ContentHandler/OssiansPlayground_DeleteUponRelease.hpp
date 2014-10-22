#pragma once;

#include <utility/utilities.hpp>
#include <cerrno>
#include <string>
#include <vector>

namespace ossianTest
{
	class Asset
	{
	public:
		Asset(std::string _name, uint _startByte, uint _size);
		~Asset();
		void SetData(void* _data);
		void* GetData();
		std::string GetName();
		uint GetSize();

	private:
		std::string name;
		uint size, startByte;
		void* data;		
	};

	class Directory
	{
	public:
		Directory(std::string _name);
		~Directory();
		void AddSubDirectory(Directory* _directory);
		void AddAsset(Asset* _asset);
		std::string GetName();

	private:
		std::vector<Directory*> subDirectories;
		std::vector<Asset*> assets;
		std::string name;

	};

	class SorryForPotato
	{
	public:
		SorryForPotato();
		~SorryForPotato();
		void ReadHeader(std::string _fileName);

	private:
		Directory* root;
		std::string fileName;

		Directory* ReadHeader(void* _data, uint& _readPointer);
		std::string ReadString(void* _data, uint& _readPointer);
		uint ReadUint(void* _data, uint& _readPointer);
	};
}