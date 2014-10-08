#pragma once
#include <cstdint>

struct DataContainer
{
	char* data;
	std::uint64_t size;

	DataContainer()
	{
		data = nullptr;
		size = 0;
	}

	DataContainer(char* _data, std::uint64_t _size)
	{
		data = _data;
		size = _size;
	}
};