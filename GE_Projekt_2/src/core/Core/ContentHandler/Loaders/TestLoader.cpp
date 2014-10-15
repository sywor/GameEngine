#include "TestLoader.hpp"



namespace trr
{

	TestLoader::TestLoader()
	{
	}

	TestLoader::~TestLoader()
	{
	}

	bool TestLoader::internal_Load(void* data, DataContainer _data)
	{
		char* test = _data.data;

		return true;
	}

	void TestLoader::internal_unload(void* data)
	{
	}
}