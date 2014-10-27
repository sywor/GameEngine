#pragma once;


#include <Core/ContentHandler/ContainerLoaders/Directory.hpp>
#include <Core/ContentHandler/ContainerLoaders/Asset.hpp>
#include <Core/ContentHandler/ContainerLoaders/PackageHandle.hpp>


typedef unsigned long dword;
typedef unsigned short word;
typedef unsigned char byte;

class ZipReader : PackageHandle
{
public:
	ZipReader(PoolAllocator* _allocator);
	~ZipReader();

	PackageResult LoadPackage(const std::string& _fileName);

protected:

	DataContainer ReadAsset(const std::string& _assetName) override;
	std::vector<DataContainer> ReadAllAssetsInDir(const std::string& _directoryName) override;

private:

	struct ZipDirEndHeader;
	struct ZipCentralDirectoryHeader;
	struct ZipLocalHeader;
	struct ZipDataDescriptor;
	struct ZipFullHeader;

	std::string fileName;
	Directory* root;
	PoolAllocator* allocator;

	PackageResult CreateSubDirectory(Directory* _root, const std::vector<std::string> _path, uint _index);
	PackageResult AddAssetToDirectory(Directory* _root, const std::vector<std::string> _path, ZipFullHeader _head, uint _index);
};