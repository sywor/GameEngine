#pragma once

#include <string>
#include "Asset.hpp"

class Directory
{
	friend class PotatoGun;
	friend class ZipReader;
	friend class PackageHandle;

public:
	Directory();
	~Directory();

private:
	Directory(std::string _name);

	void AddSubDirectory(std::string _name, Directory* _directory);
	void AddAsset(std::string _name, Asset* _asset);
	std::string GetName();
	Asset* ReadAsset(const std::vector<std::string>& _path, uint _index);
	std::map<std::string, Asset*> ReadAllAssetsInDir(const std::vector<std::string>& _path, uint _index);
	bool SubDirectoryExist(std::string _name);
	Directory* GetSubDirectory(std::string _name);

	std::map<std::string, Directory*> subDirectories;
	std::map<std::string, Asset*> assets;
	std::string name;

};