#pragma once
#include <utility/utilities.hpp>
#include <string>
#include <map>
#include <vector>

class Asset
{
	friend class Gun;

public:
	Asset();
	~Asset();

private:
	Asset(std::string _name, uint _startByte, uint _size);

	std::string GetName();
	uint GetSize();
	uint GetStart();

	std::string name;
	uint size, startByte;
};