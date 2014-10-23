#pragma once

#include <string>
#include "Asset.hpp"

namespace Potato
{
	class Directory
	{
		friend class Gun;

	public:
		Directory();
		~Directory();

	private:
		Directory(std::string _name);

		void AddSubDirectory(std::string _name, Directory* _directory);
		void AddAsset(std::string _name, Asset* _asset);
		std::string GetName();
		Asset* ReadAsset(const std::vector<std::string>& _path, uint _index);

		std::map<std::string, Directory*> subDirectories;
		std::map<std::string, Asset*> assets;
		std::string name;

	};
}