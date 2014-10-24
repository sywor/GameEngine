#pragma once

//#include <utility/utilities.hpp>
//#include <Instances/Instances.hpp>
//#include <Base/Levels/Level1.hpp>
//#include <utility/HighresTimer.hpp>
//#include <logger/Logger.hpp>
//#include <Systems/TestSystem.hpp>
//#include <Core\ContentHandler\OssiansPlaygroud_DeleteUponRelese.hpp>
//
//#include <Core\ContentHandler\ResourceManager.hpp>
//
//#include <crtdbg.h>
//
//std::wstring s2ws(const std::string& s)
//{
//	int len;
//	int slength = (int)s.length() + 1;
//	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
//	std::wstring r(len, L'\0');
//	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, &r[0], len);
//	return r;
//}
//
//std::string ws2s(const std::wstring& s)
//{
//	int len;
//	int slength = (int)s.length() + 1;
//	len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
//	std::string r(len, '\0');
//	WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, &r[0], len, 0, 0);
//	return r;
//}
//
//
//void TestScenarioA()
//{
//
//	int flag = 0;
//
//	// add a
//	trr::Entity a =  trr::entityHandler.CreateEntity();
//	trr::contentManager.GetResource("test.txt.test", [ a ]( const void* data )
//	{
//		if( trr::entityHandler.GetIndex( a ) != -1 )
//		{
//			LOG_DEBUG << a << " is alive in callback. " << std::endl;
//		}
//		else
//		{
//			LOG_DEBUG << a << " is dead in callback. " << std::endl;
//		}
//	});
//
//	// add b
//	trr::Entity b =  trr::entityHandler.CreateEntity();
//	trr::contentManager.GetResource("test.txt.test", [ b ]( const void* data )
//	{
//		if( trr::entityHandler.GetIndex( b ) != -1 )
//		{
//			LOG_DEBUG << b << " is alive in callback. " << std::endl;
//		}
//		else
//		{
//			LOG_DEBUG << b << " is dead in callback. " << std::endl;
//		}
//	});
//
//	// remove a
//	trr::entityHandler.RemoveEntity( a );
//	trr::contentManager.Unload( "test.txt.test" );
//	
//	// remove b
//	trr::entityHandler.RemoveEntity( b );
//	trr::contentManager.Unload( "test.txt.test" );
//
//	std::this_thread::sleep_for( std::chrono::milliseconds( 1300 ) );
//
//	// add c
//	trr::Entity c =  trr::entityHandler.CreateEntity();
//	trr::contentManager.GetResource("test.txt.test", [ c, &flag ]( const void* data )
//	{
//		if( trr::entityHandler.GetIndex( c ) != -1 )
//		{
//			LOG_DEBUG << c << " is alive in callback. " << std::endl;
//		}
//		else
//		{
//			LOG_DEBUG << c << " is dead in callback. " << std::endl;
//		}
//		flag = 1;
//	});
//
//	LOG_DEBUG << "main thread continues" << std::endl;
//	
//	while (flag == 0);
//	flag = 0;
//	int breakpoint = 0;
//
//	LOG_DEBUG << "TestA finished" << std::endl;
//
//}
//
//
//
//
//void TestB()
//{
//	int flag = 0;
//	trr::Entity temp;
//
//	std::vector< trr::Entity > entities;
//
//	// load level A
//	for( int i = 0; i < 100; i++ )
//	{
//		temp =  trr::entityHandler.CreateEntity();
//		trr::contentManager.GetResource("test.txt.test", [ temp ]( const void* data ) { } );
//		entities.push_back( temp );
//
//		temp =  trr::entityHandler.CreateEntity();
//		trr::contentManager.GetResource("test2.txt.test", [ temp ]( const void* data ) { } );
//		entities.push_back( temp );
//	}
//
//	//std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
//
//	// stall resource manager
//	trr::contentManager.StallLoading();
//
//	// unload level A
//	for( int i = 0; i < 100; i++ )
//	{
//		trr::contentManager.Unload( "test.txt.test" );
//		trr::contentManager.Unload( "test2.txt.test" );
//	}
//	for( int i = 0; i < entities.size(); i++ )
//	{
//		trr::entityHandler.RemoveEntity( entities[ i ] );
//	}
//	entities.clear();
//
//	// load level B
//	for( int i = 0; i < 100; i++ )
//	{
//		temp =  trr::entityHandler.CreateEntity();
//		trr::contentManager.GetResource("test2.txt.test", [ temp ]( const void* data ) { } );
//		entities.push_back( temp );
//
//		temp =  trr::entityHandler.CreateEntity();
//		trr::contentManager.GetResource("test3.txt.test", [ temp ]( const void* data ) { } );
//		entities.push_back( temp );
//	}
//
//	// resume resource manager
//	trr::contentManager.RunLoading();
//
//	std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );
//
//
//	LOG_DEBUG << "TestB finished" << std::endl;
//}
//
//
//void TestC()
//{
//	trr::contentManager.GetResource( "test.txt.test" );
//	trr::contentManager.GetResource( "test.txt" );
//	trr::contentManager.GetResource( "test.txt.notGoingToFindThisLoader" );
//	trr::contentManager.GetResource( "notGoingToFindThisFile.txt.test" );
//
//	trr::contentManager.GetResource( "test.txt.test",						[](const void* data)
//	{
//		if( CONTENT_CHECK_VALID_DATA( data ) )
//		{
//			// use
//		}
//		else
//		{
//			// error handing
//		}
//	});
//	trr::contentManager.GetResource( "test.txt",							[](const void* data){} );
//	trr::contentManager.GetResource( "test.txt.notGoingToFindThisLoader",	[](const void* data){} );
//	trr::contentManager.GetResource( "notGoingToFindThisFile.txt.test",		[](const void* data){} );
//}
//


//
//int main( int argc, char* argv[] )
//{
//	//trr::Level1 level;
//	//level.Init();
//	//ossianTest::zipTest();
//
//	//trr::contentManager.InitContentLib(s2ws("test.zip"));
//
//
//	//TestB();
//	//TestC();
//	
//
// 	//_CrtDumpMemoryLeaks();
//	//utilities::pause();
//	return 0;
//}




//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------

#include "InputManager.h"
#include "RenderInterface.h"
#include "..\gfx\DLLFactory.h"
#include "..\gfx\Graphics.h"
#include <tchar.h>

HRESULT             InitWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT				Render(float deltaTime);
HRESULT				Update(float deltaTime);

HINSTANCE				g_hInst = NULL;
HWND					g_hWnd = NULL;

RenderInterface* renderInderface;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	if (FAILED(InitWindow(hInstance, nCmdShow)))
		return 0;

	RegisterInputDevices(g_hWnd);

	renderInderface =  Renderer::RenderAPI::createRenderer(g_hWnd);

	__int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	float secsPerCnt = 1.0f / (float)cntsPerSec;

	__int64 prevTimeStamp = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);

	// Main message loop
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			__int64 currTimeStamp = 0;
			QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
			float dt = (currTimeStamp - prevTimeStamp) * secsPerCnt;

			//render
			renderInderface->update(dt);
			renderInderface->render(dt);

			renderInderface->getCamera()->update();

			prevTimeStamp = currTimeStamp;
		}
	}


	return (int)msg.wParam;
}


//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = 0;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "Base d3dx window";
	wcex.hIconSm = 0;
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	// Create window
	g_hInst = hInstance;
	RECT rc = { 0, 0, WIDTH, HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	if (!(g_hWnd = CreateWindow(
		"Base d3dx window",
		"Base d3dx window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL,
		NULL,
		hInstance,
		NULL)))
	{
		return E_FAIL;
	}

	ShowWindow(g_hWnd, nCmdShow);

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;


	case WM_INPUT:
	{
					 HandleRawInput(hWnd, (HRAWINPUT&)lParam);
	}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}


	return 0;
}
