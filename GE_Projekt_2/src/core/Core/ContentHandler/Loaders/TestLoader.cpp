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
		LOG_CONTENT << "Loading file started" << std::endl;
		void* out = m_pAllocator->FlatAllocate(in.size);
		std::memcpy( out, in.data, in.size);
		std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
		LOG_CONTENT << "Loading file finished." << std::endl;
		return out;
	}

	void TestLoader::internal_unload( void* data )
	{
		LOG_CONTENT << "Unloading file started" << std::endl;
		m_pAllocator->deallocate( data );
		std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
		LOG_CONTENT << "Unloading file finished" << std::endl;
	}

}