#pragma once;

#include <utility/utilities.hpp>
#include <string>

namespace ossianTest
{
	typedef unsigned long dword;
	typedef unsigned short word;
	typedef unsigned char byte;

	class ZipReader
	{
	public:
		ZipReader();
		~ZipReader();

		void Load(const std::string& _fileName);

	private:

		struct ZipDirEndHeader;
		struct ZipDirFileHeader;
		struct ZipLocalHeader;
		struct ZipDataDescriptor;

		std::string fileName;
	};
}