#pragma once;

#include <Core/ContentHandler/ZipHandler.hpp>
#include <utility/utilities.hpp>
#include <direct.h>
#include <cerrno>
#include <string.h>
#include <vector>

namespace ossianTest
{
	std::string removeChar(const std::string& _str, char _c)
	{
		std::vector<char> tmpCharList(_str.begin(), _str.end());
		std::vector<char> resCharList = std::vector<char>();

		for (std::vector<char>::iterator it = tmpCharList.begin(); it != tmpCharList.end(); it++)
		{
			if (*it != _c)
			{
				resCharList.push_back(*it);
			}
		}

		return std::string(resCharList.begin(), resCharList.end());
	}

	void zipTest()
	{
		ZipFile zip;

		if (!zip.Init(L"C:\\temp\\deerp\\WumpusWorld.zip"))
		{
			utilities::print("Bad Zip file");
		}
		else
		{
			unsigned int numFiles = zip.GetNumFiles();
			utilities::print("Number of files found in zip arcive: " + numFiles);

			for (int i = 0; i < numFiles; i++)
			{
				int fLen = zip.GetFileLen(i);
				std::string fName = zip.GetFilename(i);
				utilities::print("fileName: " + fName + "\t\tfileSize: " + std::to_string(fLen));

				char *pData = new char[fLen];

				if (zip.ReadFile(i, pData))
				{
					std::string filePath = "C:\\temp\\deerp\\Data\\" + fName;

					int lastIndexOfSlash = filePath.find_last_of('\\');

					if ((lastIndexOfSlash + 1) == filePath.length())
					{
						std::string tmp = removeChar(filePath.substr(0, filePath.length() - 1), '.');

						int res = _mkdir(tmp.c_str());
						if (res == -1)
						{
							utilities::print(std::string(strerror(errno)) + " : " + tmp);
						}
					}

					FILE *fo = fopen(filePath.c_str(), "wb");
					if (fo)
					{
						fwrite(pData, fLen, 1, fo);
						fclose(fo);
					}
				}

				delete[] pData;
			}

			zip.End();
		}
	}
}