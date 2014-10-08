#include "TestLoader.hpp"



namespace trr
{

	TestLoader::TestLoader()
	{
	}

	TestLoader::~TestLoader()
	{
	}

	bool TestLoader::internal_Load(std::string path, Resource& r, DataContainer _data)
	{
		char* test = _data.data;

		return false;
	}

	void TestLoader::internal_unload(Resource& r)
	{
	}
}