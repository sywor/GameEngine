#pragma once;

#include <utility/utilities.hpp>
#include <cerrno>
#include <string>
#include <map>
#include <vector>

namespace ossianTest
{
	class Asset
	{
		friend class Potato;

	public:
		Asset();
		~Asset();

	private:
		Asset(std::string _name, uint _startByte, uint _size);
		
		void* GetData();
		std::string GetName();
		uint GetSize();
		uint GetStart();

		std::string name;
		uint size, startByte;
	};

	class Directory
	{
		friend class Potato;

	public:
		Directory();
		~Directory();		

	private:
		Directory(std::string _name);
		
		void AddSubDirectory(std::string _name, Directory* _directory);
		void AddAsset(std::string _name, Asset* _asset);
		std::string GetName();
		Asset* ReadAsset(const std::vector<std::string>& _path, uint _index);

		std::map<std::string, Directory*> subDirectories;
		std::map<std::string, Asset*> assets;
		std::string name;

	};

	class Potato
	{
	public:
		Potato();
		~Potato();
		void ReadHeader(const std::string& _packageName);
		void* ReadAsset(const std::string& _fileName);

	private:
		Directory* root;
		std::string fileName;

		Directory* ReadHeader(void* _data, uint& _readPointer);
		std::string ReadString(void* _data, uint& _readPointer);
		uint ReadUint(void* _data, uint& _readPointer);
	};
}