// zLib_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MurrMurr64.hpp"
#include "zlib_impl.hpp"
#include <iostream>
#include <string>
#include <functional>
#include <chrono>
#include <random>
#include <vector>
#include <sstream>
#include <limits>
#include <direct.h>

void testHash()
{
	uint64_t loopCount = 100000;

	std::vector<std::string> data(loopCount);
	std::vector<uint64_t> stdHashData(loopCount);
	std::vector<uint64_t> MurrMurrData(loopCount);

	std::default_random_engine rnd;
	std::uniform_int_distribution<int> dist(100, 1000);
	std::uniform_int_distribution<int> charDist(33, 126);
	char rawData[1000];

	for (int i = 0; i < loopCount; i++)
	{
		uint64_t stringLenght = dist(rnd);

		for (int j = 0; j < stringLenght; j++)
		{
			rawData[j] = charDist(rnd);
		}

		data[i] = std::string(rawData, stringLenght);
	}

	std::hash<std::string> hash_fn;

	std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < loopCount; i++)
	{
		stdHashData[i] = hash_fn(data[i]);
	}
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

	uint64_t resStd = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
	std::cout << "std::hash took: " << resStd << std::endl;

	t0 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < loopCount; i++)
	{
		MurrMurrData[i] = MurmurHash64(data[i].c_str(), data[i].length(), 10);
	}
	t1 = std::chrono::high_resolution_clock::now();

	uint64_t resMurr = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();

	std::cout << "MurrMurr took: " << resMurr << "\n" << "\ndiff: " << resStd - resMurr << std::endl;
}

void generateRandomTestFile(FILE* _outFile)
{
	uint64_t loopCount = 10000000;
	std::stringstream ss;

	std::default_random_engine rnd;
	std::uniform_int_distribution<unsigned long long> charDist(0, ULLONG_MAX);

	for (int i = 0; i < loopCount; i++)
	{
		ss << charDist(rnd);
	}

	fputs(ss.str().c_str(), _outFile);
	fclose(_outFile);
}

void testCompression(std::string _experimentName)
{
	std::string rootDir = "C:/temp/test/";
	std::string fullPath = rootDir + _experimentName;

	mkdir(fullPath.c_str());

	std::string inUnStr = fullPath + "/testInUncompressed";
	std::string outComStr = fullPath + "/testOutCompressed";
	std::string outUnStr = fullPath + "/testOutUncompressed";

	FILE* inUn = fopen(inUnStr.c_str(), "w");
	FILE* outCom = fopen(outComStr.c_str(), "w");

	std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();
	generateRandomTestFile(inUn);
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

	uint64_t resStd = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
	std::cout << "generateRandomTestFile took: " << resStd << std::endl;

	inUn = fopen(inUnStr.c_str(), "r");

	SET_BINARY_MODE(inUn);
	SET_BINARY_MODE(outCom);

	zlib_impl z;

	t0 = std::chrono::high_resolution_clock::now();
	int res = z.deflate_impl(inUn, outCom);
	t1 = std::chrono::high_resolution_clock::now();
	fclose(outCom);

	assert(res == Z_OK);

	resStd = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
	std::cout << "zlib_impl::deflate_impl took: " << resStd << std::endl;

	FILE* inCom = fopen(outComStr.c_str(), "r");
	FILE* outUn = fopen(outUnStr.c_str(), "w");

	SET_BINARY_MODE(inCom);
	SET_BINARY_MODE(outUn);

	t0 = std::chrono::high_resolution_clock::now();
	res = z.inflate_impl(inCom, outUn);
	t1 = std::chrono::high_resolution_clock::now();
	fclose(outUn);

	assert(res == Z_OK);

	resStd = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
	std::cout << "zlib_impl::inflate_impl took: " << resStd << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//testHash();

	testCompression("stdCompression_unsignedLongLong");

	std::cout << "Press enter to exit the program" << std::endl;
	std::cin.get();
	return 0;
}

