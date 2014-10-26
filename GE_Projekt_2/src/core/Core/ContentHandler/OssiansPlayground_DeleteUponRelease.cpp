#include "OssiansPlayground_DeleteUponRelease.hpp"
#include <fstream>
#include <vector>

#pragma pack(1)

#pragma region Zip Headers

	struct ZipReader::ZipLocalHeader
	{
		enum
		{
			SIGNATURE = 0x04034b50,
		};

		dword		signature;
		word		version;
		word		flag;
		word		compression;		// Z_NO_COMPRESSION or Z_DEFLATED
		word		modTime;
		word		modDate;
		dword		crc32;
		dword		compressedSize;
		dword		unCompressedSize;
		word		fileNameLenght;		// Filename string follows header.
		word		extraFieldLenght;	// Extra field follows filename.
	};

	struct ZipReader::ZipDirEndHeader
	{
		enum
		{
			SIGNATURE = 0x06054b50
		};

		dword		signature;
		word		numberOfDisk;
		word		numberOfStartDisk;
		word		numberDirEntries;
		word		totalDirEntries;
		dword		dirSize;
		dword		dirOffset;
		word		commentLenght;
	};

	struct ZipReader::ZipCentralDirectoryHeader
	{
		enum
		{
			SIGNATURE = 0x02014b50
		};

		dword		signature;
		word		versionMade;
		word		versionNeeded;
		word		flag;
		word		compression;		// COMP_xxxx
		word		modTime;
		word		modDate;
		dword		crc32;
		dword		compressedSize;     // Compressed size
		dword		unCompressedSize;   // Uncompressed size
		word		fileNameLenght;     // Filename string follows header.
		word		extraLenght;        // Extra field follows filename.
		word		commentLenght;      // Comment field follows extra field.
		word		diskStart;
		word		internalAttributes;
		dword		externalAttributes;
		dword		headerOffset;

		char *GetName() const
		{
			return (char*)(this + 1);
		}

		char *GetExtra() const
		{
			return GetName() + fileNameLenght;
		}

		char *GetComment() const
		{
			return GetExtra() + extraLenght;
		}
	};

	struct ZipReader::ZipDataDescriptor
	{
		enum
		{
			SIGNATURE = 0x08074b50
		};

		dword		signature;
		dword		crc32;
		dword		compressedSize;     // Compressed size
		dword		unCompressedSize;	// Uncompressed size
	};

	struct ZipReader::ZipFullHeader
	{
		ZipCentralDirectoryHeader head;
		std::string fileName, fileCmment;
		void* extraField;
	};

#pragma endregion

	ZipReader::ZipReader(const std::string& _fileName) : fileName(_fileName), root(nullptr)
	{
		fileName = _fileName;
		ZipDirEndHeader head;


		std::ifstream fs;
		fs.open(fileName.c_str(), std::ifstream::binary);

		fs.seekg(0, std::ifstream::end);
		uint fileSize = fs.tellg();

		uint fileEndHeaderSize = sizeof(ZipDirEndHeader);
		uint centralDirectorySize = sizeof(ZipCentralDirectoryHeader);
		uint fileEndHeaderStart = fileSize - fileEndHeaderSize;

		fs.seekg(fileEndHeaderStart);
		fs.read((char*)&head, fileEndHeaderSize);

		if (head.signature != ZipDirEndHeader::SIGNATURE)
			return;

		uint centralDirectoryStart = fileEndHeaderStart - head.dirSize;

		std::vector<ZipFullHeader> assets;
		std::vector<ZipFullHeader> directories;

		fs.seekg(centralDirectoryStart);
		
		for (uint i = 0; i < head.totalDirEntries; i++)
		{
			ZipCentralDirectoryHeader central;
			ZipFullHeader fullHeader;
			fs.read((char*)&central, centralDirectorySize);

			if (central.signature != ZipCentralDirectoryHeader::SIGNATURE)
				return;

			fullHeader.head = central;

			if (central.fileNameLenght != 0)
			{
				char* buff = new char[central.fileNameLenght];
				fs.read(buff, central.fileNameLenght);
				fullHeader.fileName = std::string(buff, central.fileNameLenght);
			}

			if (central.compressedSize == 0)
				directories.push_back(fullHeader);
			else
				assets.push_back(fullHeader);

			fs.seekg(central.extraLenght + central.commentLenght, std::ifstream::cur);
		}

		for each (ZipFullHeader h in directories)//Find the root directory
		{
			std::vector<std::string> vec = utilities::split(h.fileName, '/');

			if (vec.size() == 1)
			{
				root = new Directory(vec[0]);
				break;
			}
		}	

		if (root == nullptr)
			return;

		for each(ZipFullHeader h in directories)//find all the other directories
		{
			std::vector<std::string> vec = utilities::split(h.fileName, '/');

			if (vec.size() == 1)	//we are not interested in the root directory again 
				continue;

			if (!root->SubDirectoryExist(vec[1]))
			{
				Directory* dir = new Directory(vec[1]);
				root->AddSubDirectory(vec[1], dir);
				CreateSubDirectory(dir, vec, 2);
			}
			else
			{
				CreateSubDirectory(root->GetSubDirectory(vec[1]), vec, 2);
			}			
		}

		for each (ZipFullHeader h in assets)
		{
			std::vector<std::string> vec = utilities::split(h.fileName, '/');

			AddAssetToDirectory(root, vec, h, 1);
		}
	}

	void ZipReader::CreateSubDirectory(Directory* _root, const std::vector<std::string> _path, uint _index)
	{
		if (_path.size() == _index)
		{
			return;
		}
		else if (!_root->SubDirectoryExist(_path[_index]))
		{
			Directory* dir = new Directory(_path[_index]);
			_root->AddSubDirectory(_path[_index], dir);
			CreateSubDirectory(dir, _path, _index + 1);
		}
		else
		{
			CreateSubDirectory(_root->GetSubDirectory(_path[_index]), _path, _index + 1);
		}
	}

	ZipReader::~ZipReader(){}

	void ZipReader::AddAssetToDirectory(Directory* _root, const std::vector<std::string> _path, ZipFullHeader _head, uint _index)
	{
		if (_path.size() - 1 == _index)
		{
			Asset* a = new Asset(_path[_index], _head.head.headerOffset + sizeof(ZipLocalHeader), _head.head.unCompressedSize);
			_root->AddAsset(_path[_index], a);
			return;
		}
		
		AddAssetToDirectory(_root->GetSubDirectory(_path[_index]), _path, _head, _index + 1);
	}
