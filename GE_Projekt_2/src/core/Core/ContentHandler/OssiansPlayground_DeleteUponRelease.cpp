#include "OssiansPlayground_DeleteUponRelease.hpp"
#include <fstream>
#pragma pack(1)

namespace ossianTest
{
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

	struct ZipReader::ZipDirFileHeader
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

#pragma endregion

	ZipReader::ZipReader() : fileName("N/A")
	{}

	ZipReader::~ZipReader(){}

	void ZipReader::Load(const std::string& _fileName)
	{
		fileName = _fileName;
		ZipDirEndHeader head;
		ZipDirFileHeader central;

		std::ifstream fs;
		fs.open(fileName.c_str(), std::ifstream::binary);

		fs.seekg(0, std::ifstream::end);
		uint fileSize = fs.tellg();

		uint headerSize = sizeof(ZipDirEndHeader);
		uint centralSize = sizeof(ZipDirFileHeader);
		uint headerStart = fileSize - headerSize;

		fs.seekg(headerStart);
		fs.read((char*)&head, headerSize);

		if (head.signature != ZipDirEndHeader::SIGNATURE)
			return;

		uint centralDirectoryStart = headerStart - head.dirSize;

		fs.seekg(centralDirectoryStart);
		fs.read((char*)&central, centralSize);

		fs.seekg(centralSize + central.commentLenght + central.fileNameLenght + central.extraLenght, std::ifstream::cur);

		int i = 0;

		fs.read((char*)&central, centralSize);

		int j = 0;
		
	}
}