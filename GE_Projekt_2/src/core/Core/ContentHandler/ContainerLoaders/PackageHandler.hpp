#pragma once

#include <Core/ContentHandler/ContainerLoaders/PackageHandle.hpp>

class packageHandler
{
	DataContainer ReadAsset(const std::string& _assetName);
	DataContainer ReadAllAssetsInDir(const std::string& _directoryName);
};