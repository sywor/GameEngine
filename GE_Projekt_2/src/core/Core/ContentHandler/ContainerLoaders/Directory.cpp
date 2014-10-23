#include "Directory.hpp"

namespace Potato
{
	Directory::Directory() : subDirectories(), assets()
	{
		name = "N/A";
	}

	Directory::Directory(std::string _name) : subDirectories(), assets()
	{
		name = _name;
	}

	Directory::~Directory()
	{

	}

	void Directory::AddSubDirectory(std::string _name, Directory* _directory)
	{
		subDirectories[_name] = _directory;
	}

	void Directory::AddAsset(std::string _name, Asset* _asset)
	{
		assets[_name] = _asset;
	}

	std::string Directory::GetName()
	{
		return name;
	}

	Asset* Directory::ReadAsset(const std::vector<std::string>& _path, uint _index)
	{
		if ((_path.size() - 1) == _index)
		{
			if (assets.find(_path[_index]) != assets.end())
			{
				return assets[_path[_index]];
			}

			return nullptr;
		}
		else if (subDirectories.find(_path[_index]) != subDirectories.end())
		{
			return subDirectories[_path[_index]]->ReadAsset(_path, _index + 1);
		}

		return nullptr;
	}
}