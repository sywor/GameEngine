#pragma once

#include <utility/utilities.hpp>
#include <Instances/Instances.hpp>
#include <Base/Levels/Level1.hpp>
#include <utility/HighresTimer.hpp>
#include <logger/Logger.hpp>
#include <Systems/TestSystem.hpp>
#include <Core\ContentHandler\OssiansPlaygroud_DeleteUponRelese.hpp>

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

std::string ws2s(const std::wstring& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
	std::string r(len, '\0');
	WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, &r[0], len, 0, 0);
	return r;
}


void Test_Async_Loading_Unloading()
{

	volatile int flag = 0;

	// add a
	trr::Entity a =  trr::entityHandler.CreateEntity();
	LOG_LEVEL << "loading assets for entity " << a << std::endl;
	trr::contentManager.GetResource("test.txt.test", [ a ]( const void* data )
	{
		if( trr::entityHandler.GetIndex( a ) != -1 )
		{
			LOG_CONTENT << "entity " << a << " is alive in callback. " << std::endl;
		}
		else
		{
			LOG_CONTENT << "entity " << a << " is dead in callback. " << std::endl;
		}
	});

	// add b
	trr::Entity b =  trr::entityHandler.CreateEntity();
	LOG_LEVEL << "loading assets for entity " << b << std::endl;
	trr::contentManager.GetResource("test.txt.test", [ b ]( const void* data )
	{
		if( trr::entityHandler.GetIndex( b ) != -1 )
		{
			LOG_CONTENT << "entity " << b << " is alive in callback. " << std::endl;
		}
		else
		{
			LOG_CONTENT << "entity " << b << " is dead in callback. " << std::endl;
		}
	});

	// remove a
	trr::entityHandler.RemoveEntity( a );
	LOG_LEVEL << "Unloading assets for entity " << a << std::endl;
	trr::contentManager.Unload( "test.txt.test" );
	
	// remove b
	trr::entityHandler.RemoveEntity( b );
	LOG_LEVEL << "Unloading assets for entity " << b << std::endl;
	trr::contentManager.Unload( "test.txt.test" );

	std::this_thread::sleep_for( std::chrono::milliseconds( 1300 ) );

	// add c
	trr::Entity c =  trr::entityHandler.CreateEntity();
	LOG_LEVEL << "loading assets for entity " << c << std::endl;
	trr::contentManager.GetResource("test.txt.test", [ c, &flag ]( const void* data )
	{
		if( trr::entityHandler.GetIndex( c ) != -1 )
		{
			LOG_CONTENT << "entity " << c << " is alive in callback. " << std::endl;
		}
		else
		{
			LOG_CONTENT << "entity " << c << " is dead in callback. " << std::endl;
		}
		flag = 1;
	});

	LOG_LEVEL << "main thread continues" << std::endl;
	
	while (flag == 0);
	flag = 0;
	int breakpoint = 0;

	trr::contentManager.DumpAssetList();

	trr::contentManager.Unload("test.txt.test");
	// wait for unload to finish before next test.
	std::this_thread::sleep_for(std::chrono::milliseconds(1300));

	LOG_DEBUG << "Async_Loading_Unloading test finished" << std::endl;
}




void Test_Level_Loading_Unloading()
{
	volatile int flag = 0;
	trr::Entity temp;

	std::vector< trr::Entity > entities;

	// load level A
	LOG_LEVEL << "Loading level A" << std::endl;
	for( int i = 0; i < 100; i++ )
	{
		temp =  trr::entityHandler.CreateEntity();
		trr::contentManager.GetResource("test.txt.test", [ temp ]( const void* data ) { } );
		entities.push_back( temp );

		temp =  trr::entityHandler.CreateEntity();
		trr::contentManager.GetResource("test2.txt.test", [ temp ]( const void* data ) { } );
		entities.push_back( temp );
	}

	std::this_thread::sleep_for( std::chrono::milliseconds( 1200 ) );
	trr::contentManager.DumpAssetList();

	// stall resource manager
	trr::contentManager.StallLoading();

	// unload level A
	LOG_LEVEL << "Unload level A" << std::endl;
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
	LOG_LEVEL << "Loading level B" << std::endl;
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

	std::this_thread::sleep_for(std::chrono::milliseconds(1200));
	trr::contentManager.DumpAssetList();

	// unload level B
	LOG_LEVEL << "Unload level B" << std::endl;
	for (int i = 0; i < 100; i++)
	{
		trr::contentManager.Unload("test2.txt.test");
		trr::contentManager.Unload("test3.txt.test");
	}
	for (int i = 0; i < entities.size(); i++)
	{
		trr::entityHandler.RemoveEntity(entities[i]);
	}
	entities.clear();

	std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );


	LOG_LEVEL << "Level_Loading_Unloading test finished" << std::endl;
}

void CheckData(std::string path, const void* data)
{
	if (CONTENT_CHECK_VALID_DATA(data))
	{
		LOG_CONTENT << path << " was loaded correctly" << std::endl;
	}
	else
	{
		if (data == CONTENT_CALLBACK_CAN_NOT_FIND_FILE)
		{
			LOG_CONTENT << path << " : could not find file." << std::endl;
		}
		else if (data == CONTENT_CALLBACK_LOADING_FAILED)
		{
			LOG_CONTENT << path << " : loading failed" << std::endl;
		}
		else if (data == CONTENT_CALLBACK_NO_LOADER_ACCEPTS_FILE)
		{
			LOG_CONTENT << path << " : format not supported" << std::endl;
		}
		else if (data == CONTENT_CALLBACK_OUT_OF_MEMORY)
		{
			LOG_CONTENT << path << " : content system is out of memory" << std::endl;
		}
	}
}


void Test_Error_Handling()
{
	LOG_LEVEL << "Loading " << "test.txt.test" << std::endl;
	trr::Resource a = trr::contentManager.GetResource( "test.txt.test" );

	LOG_LEVEL << "Loading " << "test.txt" << std::endl;
	trr::Resource b = trr::contentManager.GetResource( "test.txt" );

	LOG_LEVEL << "Loading " << "test.txt.notGoingToFindThisLoader" << std::endl;
	trr::Resource c = trr::contentManager.GetResource( "test.txt.notGoingToFindThisLoader" );

	LOG_LEVEL << "Loading " << "notGoingToFindThisFile.txt.test" << std::endl;
	trr::Resource d = trr::contentManager.GetResource( "notGoingToFindThisFile.txt.test" );

	
	CheckData( "test.txt.test", a.getData());
	CheckData( "test.txt", b.getData());
	CheckData( "test.txt.notGoingToFindThisLoader", c.getData());
	CheckData( "notGoingToFindThisFile.txt.test", d.getData());

	LOG_LEVEL << "Level_Loading_Unloading test finished" << std::endl;
}




int main( int argc, char* argv[] )
{
	//trr::Level1 level;
	//level.Init();
	//ossianTest::zipTest();

	trr::contentManager.InitContentLib(s2ws("test.zip"));

	Test_Async_Loading_Unloading();
	LOG_DEBUG << std::endl;
	Test_Level_Loading_Unloading();
	LOG_DEBUG << std::endl;
	Test_Error_Handling();
	
	//TestB();
	//TestC();
	

 	_CrtDumpMemoryLeaks();
	//utilities::pause();
	return 0;
}



