#pragma once
#include <cstdint>
#include <Core/ContentHandler/ContainerLoaders/PackageResult.hpp>
#include <utility/utilities.hpp>

struct DataContainer
{
	char* data;
	uint size;
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

	DataContainer(char* _data, uint _size)
	{
		data = _data;
		size = _size;
		err = PackageResult::PACKAGE_OK;
	}
};