#pragma once
#include <cstdint>
#include <Core/ContentHandler/ContainerLoaders/PackageResult.hpp>

struct DataContainer
{
	char* data;
	std::uint64_t size;
	PackageResult err;

	DataContainer()
	{
		data = nullptr;
		size = 0;
		err = PackageResult::PACKAGE_OK;
	}

	DataContainer(PackageResult _err)
	{
		data = nullptr;
		size = 0;
		err = _err;
	}

	DataContainer(char* _data, std::uint64_t _size)
	{
		data = _data;
		size = _size;
		err = PackageResult::PACKAGE_OK;
	}
};