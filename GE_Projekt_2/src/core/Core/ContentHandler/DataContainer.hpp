#pragma once
#include <cstdint>

struct DataContainer
{
	char* data;
	std::uint64_t size;
	std::string	tag;

	DataContainer()
	{
		data = nullptr;
		size = 0;
		tag	 = "no_tag";
	}

	DataContainer(char* _data, std::uint64_t _size, std::string _tag = "")
		: tag(_tag)
	{
		data = _data;
		size = _size;
	}
};