//#include "stdafx.h"
//
//#include "InputManager.h"
//#include "ButtonInput.h"
//#include <vector>
//#include "Graphics.h"
//#include "XMCamera.h"
//
//
////--------------------------------------------------------------------------------------
//// Forward declarations
////--------------------------------------------------------------------------------------
//HRESULT             InitWindow(HINSTANCE hInstance, int nCmdShow);
//LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
//HRESULT				Render(float deltaTime);
//HRESULT				Update(float deltaTime);
//
//HINSTANCE				g_hInst = NULL;
//HWND					g_hWnd = NULL;
//
//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
//{
//	if (FAILED(InitWindow(hInstance, nCmdShow)))
//		return 0;
//
//	RegisterInputDevices(g_hWnd);
//
//	__int64 cntsPerSec = 0;
//	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
//	float secsPerCnt = 1.0f / (float)cntsPerSec;
//
//	__int64 prevTimeStamp = 0;
//	QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);
//
//	// Main message loop
//	MSG msg = { 0 };
//	while (WM_QUIT != msg.message)
//	{
//		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
//		{
//			TranslateMessage(&msg);
//			DispatchMessage(&msg);
//		}
//		else
//		{
//			__int64 currTimeStamp = 0;
//			QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
//			float dt = (currTimeStamp - prevTimeStamp) * secsPerCnt;
//
//			//render
//			graphics->Update(dt);
//			graphics->Render(dt);
//
//			Cam->update();
//
//			prevTimeStamp = currTimeStamp;
//		}
//	}
//
//	return (int)msg.wParam;
//}
//
//
////--------------------------------------------------------------------------------------
//// Register class and create window
////--------------------------------------------------------------------------------------
//HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow)
//{
//	// Register class
//	WNDCLASSEX wcex;
//	wcex.cbSize = sizeof(WNDCLASSEX);
//	wcex.style = CS_HREDRAW | CS_VREDRAW;
//	wcex.lpfnWndProc = WndProc;
//	wcex.cbClsExtra = 0;
//	wcex.cbWndExtra = 0;
//	wcex.hInstance = hInstance;
//	wcex.hIcon = 0;
//	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
//	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//	wcex.lpszMenuName = NULL;
//	wcex.lpszClassName = "Base d3dx window";
//	wcex.hIconSm = 0;
//	if (!RegisterClassEx(&wcex))
//		return E_FAIL;
//
//	// Create window
//	g_hInst = hInstance;
//	RECT rc = { 0, 0, WIDTH, HEIGHT };
//	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
//
//	if (!(g_hWnd = CreateWindow(
//		"Base d3dx window",
//		"Base d3dx window",
//		WS_OVERLAPPEDWINDOW,
//		CW_USEDEFAULT,
//		CW_USEDEFAULT,
//		rc.right - rc.left,
//		rc.bottom - rc.top,
//		NULL,
//		NULL,
//		hInstance,
//		NULL)))
//	{
//		return E_FAIL;
//	}
//
//	ShowWindow(g_hWnd, nCmdShow);
//
//	return S_OK;
//}
//
//
////--------------------------------------------------------------------------------------
//// Called every time the application receives a message
////--------------------------------------------------------------------------------------
//
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	PAINTSTRUCT ps;
//	HDC hdc;
//
//	switch (message)
//	{
//	case WM_PAINT:
//		hdc = BeginPaint(hWnd, &ps);
//		EndPaint(hWnd, &ps);
//		break;
//
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		break;
//
//
//	case WM_INPUT:
//	{
//		HandleRawInput(hWnd, (HRAWINPUT&)lParam);
//	}
//		break;
//
//	default:
//		return DefWindowProc(hWnd, message, wParam, lParam);
//	}
//
//	return 0;
//}