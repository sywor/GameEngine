#pragma once

#include <utility/utilities.hpp>
#include <Core/Memory/PoolAllocator.h>
#include <Core/ContentHandler/ContainerLoaders/Directory.hpp>
#include <Core/ContentHandler/ContainerLoaders/Asset.hpp>
#include <Core/ContentHandler/ContainerLoaders/DataContainer.hpp>
#include <string>
#include <fstream>
#include <sstream>

class PackageHandle
{
public:

	PackageHandle(PoolAllocator* _allocator);

	DataContainer ReadAsset(const std::string& _assetName);	

	std::vector<DataContainer> ReadAllAssetsInDir(const std::string& _directoryName);
	std::string GetName();

protected:

	virtual PackageResult LoadPackage(const std::string& _packageName) = 0;

	std::vector<std::string> SplitPathName(const std::string& _assetName);

	Directory* root;
	std::string packageName;
	PoolAllocator* allocator;	
};