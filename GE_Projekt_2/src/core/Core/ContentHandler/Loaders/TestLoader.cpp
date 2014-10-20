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

	void* TestLoader::internal_Load( DataContainer in)
	{
		void* out = m_pAllocator->FlatAllocate(in.size);
		std::memcpy( out, in.data, in.size);

		std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

		return out;
	}

	void TestLoader::internal_unload(void** data)
	{
	}
}