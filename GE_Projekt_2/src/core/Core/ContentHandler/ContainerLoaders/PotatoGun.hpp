#pragma once

#include <Core/ContentHandler/ContainerLoaders/PackageHandle.hpp>

class PotatoGun : PackageHandle
{
public:
	PotatoGun(PoolAllocator* _allocator);
	~PotatoGun();

	PackageResult LoadPackage(const std::string& _packageName);

protected:

	DataContainer ReadAsset(const std::string& _assetName) override;
	std::vector<DataContainer> ReadAllAssetsInDir(const std::string& _directoryName) override;

private:
	Directory* root;
	std::string packageName;
	PoolAllocator* allocator;

	PackageResult ReadHeader(void* _data, uint& _readPointer, Directory* _root);
	std::string ReadString(void* _data, uint& _readPointer);
	uint ReadUint(void* _data, uint& _readPointer);
};