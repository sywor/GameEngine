#pragma once

#include <utility/utilities.hpp>
#include <Core/Memory/PoolAllocator.h>
#include <Core/ContentHandler/ContainerLoaders/Directory.hpp>
#include <Core/ContentHandler/ContainerLoaders/DataContainer.hpp>
#include <string>

class PackageHandle
{
	virtual DataContainer ReadAsset(const std::string& _assetName) = 0;
	virtual std::vector<DataContainer> ReadAllAssetsInDir(const std::string& _directoryName) = 0;
};