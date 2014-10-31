#include "PackageHandler.hpp"

packageHandler::packageHandler()
{

}

packageHandler::~packageHandler()
{

}

void packageHandler::AddHandle(PackageHandle* _handle)
{
	handles[_handle->GetName()] = _handle;
}

DataContainer packageHandler::ReadAsset(const std::string& _assetName)
{
	std::map<std::string, PackageHandle*>::iterator it;
	DataContainer result = DataContainer(PackageResult::FILE_NOT_FOUND);

	for (it = handles.begin(); it != handles.end(); it++)
	{
		result = it->second->ReadAsset(_assetName);

		if (result.err == PackageResult::PACKAGE_OK || result.err == PackageResult::OUT_OF_MEMORY)
		{
			return result;
		}
	}

	return result;
}

DataContainer packageHandler::ReadAsset(const std::string& _assetName, std::string _packageName)
{
	if (handles.find(_packageName) != handles.end())
	{
		return handles[_packageName]->ReadAsset(_assetName);
	}

	return DataContainer(PackageResult::FILE_NOT_FOUND);
}

std::vector<DataContainer> packageHandler::ReadAllAssetsInDir(const std::string& _directoryName)
{
	std::map<std::string, PackageHandle*>::iterator it;
	std::vector<DataContainer> result;

	for (it = handles.begin(); it != handles.end(); it++)
	{
		result = it->second->ReadAllAssetsInDir(_directoryName);

		if (result[0].err == PackageResult::PACKAGE_OK || result[0].err == PackageResult::OUT_OF_MEMORY)
		{
			return result;
		}
	}

	return result;
}

std::vector<DataContainer> packageHandler::ReadAllAssetsInDir(const std::string& _directoryName, std::string _packageName)
{
	std::vector<DataContainer> result;

	if (handles.find(_packageName) != handles.end())
	{
		result = handles[_packageName]->ReadAllAssetsInDir(_directoryName);
	}
	else
	{
		result.push_back(DataContainer(PackageResult::DIRECTORY_NOT_FOUND));
	}	
	
	return result;
}