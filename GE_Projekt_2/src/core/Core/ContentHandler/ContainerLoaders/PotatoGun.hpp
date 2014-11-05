#pragma once

#include <Core/ContentHandler/ContainerLoaders/PackageHandle.hpp>

class PotatoGun : public PackageHandle
{
public:
	PotatoGun(PoolAllocator* _allocator);
	~PotatoGun();

	PackageResult LoadPackage(const std::string& _packageName) override;

private:

	PackageResult ReadHeader(void* _data, uint& _readPointer, Directory*& _root);
	std::string ReadString(void* _data, uint& _readPointer);
	uint ReadUint(void* _data, uint& _readPointer);
};