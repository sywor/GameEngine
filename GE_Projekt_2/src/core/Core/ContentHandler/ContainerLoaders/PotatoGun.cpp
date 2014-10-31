#include <Core\ContentHandler\ResourceManager.hpp>
#include "PotatoGun.hpp"

PotatoGun::PotatoGun(PoolAllocator* _allocator) : PackageHandle(_allocator)
{

}

PotatoGun::~PotatoGun()
{

}

PackageResult PotatoGun::LoadPackage(const std::string& _packageName)
{
	packageName = _packageName;

	std::ifstream fs;
	fs.open(packageName.c_str(), std::ifstream::binary);

	if (fs.is_open())
	{			
		uint headerStart;
		fs.read((char*)&headerStart, sizeof(uint));
		fs.seekg(0, std::ifstream::end);
		uint fileEnd = fs.tellg();
		uint headerSize = fileEnd - headerStart;
		fs.seekg(headerStart);
		char* buffer = allocator->allocate<char>(headerSize);

		if (buffer == nullptr)
			return PackageResult::OUT_OF_MEMORY;

		fs.read(buffer, headerSize);

		uint readPointer = 0;
		PackageResult err = ReadHeader(buffer, readPointer, root);

		allocator->deallocate(buffer);
		fs.close();

		if (err != PackageResult::PACKAGE_OK)
		{
			allocator->deallocate(root);
			root = nullptr;
			return err;
		}			

		return PackageResult::PACKAGE_OK;
	}
	
	return PackageResult::FILE_NOT_FOUND;
}

PackageResult PotatoGun::ReadHeader(void* _data, uint& _readPointer, Directory* _root)
{
	std::string directoryName = ReadString(_data, _readPointer);
	uint directoryAssetCount = ReadUint(_data, _readPointer);

	Directory* dt = (Directory*)allocator->FlatAllocate(sizeof(Directory));

	if (dt == nullptr)
		return PackageResult::OUT_OF_MEMORY;

	_root = new(dt)Directory(directoryName);

	for (uint i = 0; i < directoryAssetCount; i++)
	{
		std::string name = ReadString(_data, _readPointer);
		uint start = ReadUint(_data, _readPointer);
		uint size = ReadUint(_data, _readPointer);

		Asset* at = (Asset*)allocator->FlatAllocate(sizeof(Asset));

		if (at == nullptr)
			return PackageResult::OUT_OF_MEMORY;

		Asset* a = new(at) Asset(name, start, size);
		_root->AddAsset(name, a);
	}

	uint subDirectoryCount = ReadUint(_data, _readPointer);

	for (uint i = 0; i < subDirectoryCount; i++)
	{
		Directory* tmp;
		PackageResult err = ReadHeader(_data, _readPointer, tmp);

		if (err != PackageResult::PACKAGE_OK)
			return err;

		_root->AddSubDirectory(tmp->GetName(), tmp);
	}

	return PackageResult::PACKAGE_OK;
}

std::string PotatoGun::ReadString(void* _data, uint& _readPointer)
{
	char* data = (char*)_data;
	data = data + _readPointer;
	uint size;
	memcpy(&size, data, sizeof(uint));
	std::string str(data + sizeof(uint), size);
	_readPointer += sizeof(uint)+size;

	return str;
}

uint PotatoGun::ReadUint(void* _data, uint& _readPointer)
{
	char* data = (char*)_data;
	data = data + _readPointer;
	uint result;
	memcpy(&result, data, sizeof(uint));
	_readPointer += sizeof(uint);

	return result;
}