#pragma once;

#include <utility/utilities.hpp>
#include <string>
#include "Core/ContentHandler/ContainerLoaders/Directory.hpp"
#include "Core/ContentHandler/ContainerLoaders/Asset.hpp"


typedef unsigned long dword;
typedef unsigned short word;
typedef unsigned char byte;

class ZipReader
{
public:
	ZipReader(const std::string& _fileName);
	~ZipReader();

private:

	struct ZipDirEndHeader;
	struct ZipCentralDirectoryHeader;
	struct ZipLocalHeader;
	struct ZipDataDescriptor;
	struct ZipFullHeader;

	std::string fileName;
	Directory* root;

	void CreateSubDirectory(Directory* _root, const std::vector<std::string> _path, uint _index);
	void AddAssetToDirectory(Directory* _root, const std::vector<std::string> _path, ZipFullHeader _head, uint _index);
};