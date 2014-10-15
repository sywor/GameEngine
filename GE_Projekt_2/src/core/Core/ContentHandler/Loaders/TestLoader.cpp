#include "TestLoader.hpp"
#include <thread>


namespace trr
{

	TestLoader::TestLoader()
	{
	}

	TestLoader::~TestLoader()
	{
	}

	bool TestLoader::internal_Load(void** out, DataContainer in)
	{
		*out = m_pAllocator->FlatAllocate(in.size);
		std::memcpy(*out, in.data, in.size);

		std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

		return true;
	}

	void TestLoader::internal_unload(void** data)
	{
	}
}