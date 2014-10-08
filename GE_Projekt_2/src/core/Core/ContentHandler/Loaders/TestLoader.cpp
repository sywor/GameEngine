#include "TestLoader.hpp"



namespace trr
{

	TestLoader::TestLoader()
	{
	}

	TestLoader::~TestLoader()
	{
	}

	bool TestLoader::internal_Load(std::string path, Resource& r)
	{
		return false;
	}

	void TestLoader::internal_unload(Resource& r)
	{
	}
}