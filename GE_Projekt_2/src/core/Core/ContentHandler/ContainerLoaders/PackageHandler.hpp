#pragma once

#include <Core/ContentHandler/ContainerLoaders/PackageHandle.hpp>

class packageHandler
{
public:
	packageHandler();
	~packageHandler();

	void AddHandle(PackageHandle* _handle);
	DataContainer ReadAsset(const std::string& _assetName);
	DataContainer ReadAsset(const std::string& _assetName, std::string _packageName);
	std::vector<DataContainer> ReadAllAssetsInDir(const std::string& _directoryName);
	std::vector<DataContainer> ReadAllAssetsInDir(const std::string& _directoryName, std::string _packageName);

private:
	
	std::map<std::string, PackageHandle*> handles;
};