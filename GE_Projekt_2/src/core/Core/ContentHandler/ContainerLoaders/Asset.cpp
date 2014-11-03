#include "Asset.hpp"

Asset::Asset() : name("N/A"), startByte(0), size(0)
{
}

Asset::Asset(std::string _name, uint _startByte, uint _size) : name(_name), startByte(_startByte), size(_size)
{
}

Asset::~Asset()
{

}

std::string Asset::GetName()
{
	return name;
}

uint Asset::GetSize()
{
	return size;
}

uint Asset::GetStart()
{
	return startByte;
}