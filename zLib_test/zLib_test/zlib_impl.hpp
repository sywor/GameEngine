#pragma once

#include "stdafx.h"
#include <string>
#include <assert.h>
#include "zlib\include\zlib.h"

#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

#define CHUNK 262144
#define COMPRESSION_LEVEL Z_DEFAULT_COMPRESSION

class zlib_impl
{
public:
	zlib_impl(){};

	int deflate_impl(FILE *source, FILE *dest);
	int inflate_impl(FILE *source, FILE *dest);
	void zerr_impl(int _err);
};