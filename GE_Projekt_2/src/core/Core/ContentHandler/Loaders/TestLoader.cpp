#include "TestLoader.hpp"
#include <thread>

#include <logger/Logger.hpp>

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

		std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

		LOG_DEBUG << "loaded test file" << std::endl;

		return out;
	}

	void TestLoader::internal_unload( void* data )
	{
		m_pAllocator->deallocate( data );

		std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
		LOG_DEBUG << "unloaded test file" << std::endl;
	}

}