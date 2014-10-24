#pragma once
// --------------------------------------------------------------------------
// File:        ZipFile.h
//
// Purpose:     The declaration of a quick'n dirty ZIP file reader class.
//
// --------------------------------------------------------------------------

#include <stdio.h>
#include <string>
#include <map>
#include <algorithm>
#include <Core\Memory\PoolAllocator.h>

#define DELETE_ARRAY(x) if(x) delete[] x; x = NULL;

typedef std::map<std::string, int> ZipContentsMap;		// maps path to a zip content id

class ZipFile
{
private:
	struct TZipDirHeader;
	struct TZipDirFileHeader;
	struct TZipLocalHeader;

	FILE*				m_pFile;	// Zip file
	char*				m_pDirData;	// Raw data buffer.
	int					m_nEntries;	// Number of entries.
	PoolAllocator*		allocator;

	// Pointers to the dir entries in pDirData.
	const TZipDirFileHeader** m_papDir;

	int Find(const std::string &path) const;
	int GetFileLen(int i) const;

public:
	ZipFile(PoolAllocator* _allocator) 
	{ 
		m_nEntries = 0; 
		m_pFile = NULL; 
		m_pDirData = NULL; 
		allocator = _allocator;
	}

	ZipFile()
	{
		m_nEntries = 0;
		m_pFile = NULL;
		m_pDirData = NULL;
		allocator = nullptr;
	}

	virtual ~ZipFile() { End(); if(m_pFile)fclose(m_pFile); }

	bool Load(const std::wstring &resFileName);
	void End();

	//int GetNumFiles()const { return m_nEntries; }
	//std::string GetFilename(int i) const;	
	void* ReadFile(const std::string &path);
	// Added to show multi-threaded decompression
	//bool ReadLargeFile(int i, void *pBuf, void(*progressCallback)(int, bool &));	

	ZipContentsMap m_ZipContentsMap;
};

