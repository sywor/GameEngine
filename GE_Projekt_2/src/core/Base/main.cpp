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




int main( int argc, char* argv[] )
{
	//trr::Level1 level;
	//level.Init();
	//ossianTest::zipTest();

	int flag = 0;

	trr::contentManager.InitContentLib(s2ws("test.zip"));

	

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

 	_CrtDumpMemoryLeaks();
	//utilities::pause();
	return 0;
}



