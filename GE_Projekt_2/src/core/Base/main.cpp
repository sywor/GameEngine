#pragma once

#include <utility/utilities.hpp>
#include <Instances/Instances.hpp>
#include <Base/Levels/Level1.hpp>
#include <utility/HighresTimer.hpp>
#include <logger/Logger.hpp>
#include <Systems/TestSystem.hpp>
//#include <Core\ContentHandler\OssiansPlayground_DeleteUponRelease.hpp>
#include <Core\ContentHandler\ContainerLoaders\Gun.hpp>

#include <Core\ContentHandler\ResourceManager.hpp>

#include <crtdbg.h>

std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	std::wstring r(len, L'\0');
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, &r[0], len);
	return r;
}

void TestScenarioA()
{

	int flag = 0;

	// add a
	trr::Entity a =  trr::entityHandler.CreateEntity();
	trr::contentManager.GetResource("test.txt.test", [ a ]( const void* data )
	{
		if( trr::entityHandler.GetIndex( a ) != -1 )
		{
			LOG_DEBUG << a << " is alive in callback. " << std::endl;
		}
		else
		{
			LOG_DEBUG << a << " is dead in callback. " << std::endl;
		}
	});

	// add b
	trr::Entity b =  trr::entityHandler.CreateEntity();
	trr::contentManager.GetResource("test.txt.test", [ b ]( const void* data )
	{
		if( trr::entityHandler.GetIndex( b ) != -1 )
		{
			LOG_DEBUG << b << " is alive in callback. " << std::endl;
		}
		else
		{
			LOG_DEBUG << b << " is dead in callback. " << std::endl;
		}
	});

	// remove a
	trr::entityHandler.RemoveEntity( a );
	trr::contentManager.Unload( "test.txt.test" );
	
	// remove b
	trr::entityHandler.RemoveEntity( b );
	trr::contentManager.Unload( "test.txt.test" );

	std::this_thread::sleep_for( std::chrono::milliseconds( 1300 ) );

	// add c
	trr::Entity c =  trr::entityHandler.CreateEntity();
	trr::contentManager.GetResource("test.txt.test", [ c, &flag ]( const void* data )
	{
		if( trr::entityHandler.GetIndex( c ) != -1 )
		{
			LOG_DEBUG << c << " is alive in callback. " << std::endl;
		}
		else
		{
			LOG_DEBUG << c << " is dead in callback. " << std::endl;
		}
		flag = 1;
	});

	LOG_DEBUG << "main thread continues" << std::endl;
	
	while (flag == 0);
	flag = 0;
	int breakpoint = 0;

	LOG_DEBUG << "TestA finished" << std::endl;

}

void TestB()
{
	int flag = 0;
	trr::Entity temp;

	std::vector< trr::Entity > entities;

	// load level A
	for( int i = 0; i < 100; i++ )
	{
		temp =  trr::entityHandler.CreateEntity();
		trr::contentManager.GetResource("test.txt.test", [ temp ]( const void* data ) { } );
		entities.push_back( temp );

		temp =  trr::entityHandler.CreateEntity();
		trr::contentManager.GetResource("test2.txt.test", [ temp ]( const void* data ) { } );
		entities.push_back( temp );
	}

	//std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

	// stall resource manager
	trr::contentManager.StallLoading();

	// unload level A
	for( int i = 0; i < 100; i++ )
	{
		trr::contentManager.Unload( "test.txt.test" );
		trr::contentManager.Unload( "test2.txt.test" );
	}
	for( int i = 0; i < entities.size(); i++ )
	{
		trr::entityHandler.RemoveEntity( entities[ i ] );
	}
	entities.clear();

	// load level B
	for( int i = 0; i < 100; i++ )
	{
		temp =  trr::entityHandler.CreateEntity();
		trr::contentManager.GetResource("test2.txt.test", [ temp ]( const void* data ) { } );
		entities.push_back( temp );

		temp =  trr::entityHandler.CreateEntity();
		trr::contentManager.GetResource("test3.txt.test", [ temp ]( const void* data ) { } );
		entities.push_back( temp );
	}

	// resume resource manager
	trr::contentManager.RunLoading();

	std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );


	LOG_DEBUG << "TestB finished" << std::endl;
}

void TestC()
{
	trr::contentManager.GetResource( "test.txt.test" );
	trr::contentManager.GetResource( "test.txt" );
	trr::contentManager.GetResource( "test.txt.notGoingToFindThisLoader" );
	trr::contentManager.GetResource( "notGoingToFindThisFile.txt.test" );

	trr::contentManager.GetResource( "test.txt.test",						[](const void* data)
	{
		if( CONTENT_CHECK_VALID_DATA( data ) )
		{
			// use
		}
		else
		{
			// error handing
		}
	});
	trr::contentManager.GetResource( "test.txt",							[](const void* data){} );
	trr::contentManager.GetResource( "test.txt.notGoingToFindThisLoader",	[](const void* data){} );
	trr::contentManager.GetResource( "notGoingToFindThisFile.txt.test",		[](const void* data){} );
}


int main( int argc, char* argv[] )
{
	//trr::Level1 level;
	//level.Init();
	//ossianTest::zipTest();

	//trr::contentManager.InitContentLib(s2ws("test.zip"));


	//TestB();
	//TestC();
	//PoolAllocator* allocator;

	//Potato::Gun gun(allocator);
	//gun.Load("zlib128-dll.Spud");	
	//void* v = gun.Shoot("test/minigzip_d.exe");

 	_CrtDumpMemoryLeaks();
	//utilities::pause();
	return 0;
}



