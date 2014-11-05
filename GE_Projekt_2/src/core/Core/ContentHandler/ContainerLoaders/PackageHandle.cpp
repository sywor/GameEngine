#include <Core/ContentHandler/ContainerLoaders/PackageHandle.hpp>

PackageHandle::PackageHandle(PoolAllocator* _allocator) : allocator(_allocator), root(nullptr), packageName("N/A")
{

}

DataContainer PackageHandle::ReadAsset(const std::string& _assetName)
{
	std::ifstream fs;
	std::vector<std::string> path = SplitPathName(_assetName);

	Asset* a = root->ReadAsset(path, 0);

	if (a == nullptr)
		return DataContainer(PackageResult::FILE_NOT_FOUND);

	fs.open(packageName.c_str(), std::ifstream::binary);
	char* result = (char*)allocator->FlatAllocate(a->GetSize());

	if (result == nullptr)
		return DataContainer(PackageResult::OUT_OF_MEMORY);

	fs.seekg(a->GetStart());
	fs.read(result, a->GetSize());
	fs.close();

	return DataContainer(result, a->GetSize());
}

std::vector<DataContainer> PackageHandle::ReadAllAssetsInDir(const std::string& _directoryName)
{
	std::vector<DataContainer> result;
	std::ifstream fs;
	std::vector<std::string> path = SplitPathName(_directoryName);
	std::map<std::string, Asset*>::iterator it;

	std::map<std::string, Asset*> amap = root->ReadAllAssetsInDir(path, 0);

	if (amap.size() == 0)
	{
		result.push_back(DataContainer(PackageResult::DIRECTORY_NOT_FOUND));
		return result;
	}

	fs.open(packageName.c_str(), std::ifstream::binary);

	for (it = amap.begin(); it != amap.end(); it++)
	{
		char* buffer = (char*)allocator->FlatAllocate(it->second->GetSize());

		if (buffer == nullptr)
		{
			result.push_back(DataContainer(PackageResult::OUT_OF_MEMORY));
			return result;
		}

		fs.seekg(it->second->GetStart());
		fs.read(buffer, it->second->GetSize());
		result.push_back(DataContainer(buffer, it->second->GetSize()));
	}

	fs.close();

	return result;
}

std::string PackageHandle::GetName()
{
	return packageName;
}

std::vector<std::string> PackageHandle::SplitPathName(const std::string& _assetName)
{
	std::vector<std::string> path;
	std::stringstream ss(_assetName);
	std::string item;	

	while (std::getline(ss, item, '/'))
	{
		path.push_back(item);
	}

	return path;
}
