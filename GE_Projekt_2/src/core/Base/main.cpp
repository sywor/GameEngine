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


	trr::contentManager.GetResource("test.txt.test", [ &flag ]( void* data )
	{
		static std::mutex tex;
		char code[10];
		std::memcpy( code, data, 10 );
		LOG_DEBUG << "callback: " << code << std::endl;

		trr::Entity ent = trr::entityHandler.CreateEntity();
		trr::entityHandler.AddComponent< trr::TransformationComponent >( ent );

		flag = 1;
	});


	LOG_DEBUG << "main thread continues" << std::endl;
	
	//trr::contentManager.InitContentLib(s2ws("C:/temp/GameEngine/GE_Projekt_2/test.zip"));
	//trr::Resource r = trr::contentManager.GetResource("test.txt.test");
	//trr::Resource rr = trr::contentManager.GetResource("test.txt.test");
	//
	//trr::contentManager.Unload(r.hash);
	//trr::contentManager.Unload(rr.hash);
		
	while (flag == 0)
	{
		// empty
	}

	int breakpoint = 0;

 	_CrtDumpMemoryLeaks();
	//utilities::pause();
	return 0;
}



