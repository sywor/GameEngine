#pragma once
#include <string>

#define PR_TOSTRING(x) #x

enum class PackageResult
{
	PACKAGE_OK,
	OUT_OF_MEMORY,
	FILE_NOT_FOUND,
	DIRECTORY_NOT_FOUND,
	ZIP_INVALID_END_HEADER,
	ZIP_INVALID_CENTRALDIR_HEADER,
	ZIP_ROOTDIR_NOT_FOUND
};

static std::string PackageResultToString(PackageResult _pr)
{
	switch (_pr)
	{
	case PackageResult::PACKAGE_OK:
		return PR_TOSTRING(PACKAGE_OK);

	case PackageResult::OUT_OF_MEMORY:
		return PR_TOSTRING(OUT_OF_MEMORY);

	case PackageResult::FILE_NOT_FOUND:
		return PR_TOSTRING(FILE_NOT_FOUND);

	case PackageResult::DIRECTORY_NOT_FOUND:
		return PR_TOSTRING(DIRECTORY_NOT_FOUND);

	case PackageResult::ZIP_INVALID_END_HEADER:
		return PR_TOSTRING(ZIP_INVALID_END_HEADER);

	case PackageResult::ZIP_INVALID_CENTRALDIR_HEADER:
		return PR_TOSTRING(ZIP_INVALID_CENTRALDIR_HEADER);

	case PackageResult::ZIP_ROOTDIR_NOT_FOUND:
		return PR_TOSTRING(ZIP_ROOTDIR_NOT_FOUND);
	}
}