#pragma once
#include <iostream>
#include <limits>

typedef unsigned int uint;

namespace utilities
{
	static const float pi = 3.14159265359f;

	static void pause()
	{
		std::cout << "Press \"the\" any key twice to close the program" << std::endl;
		std::cin.get();		
	}

	static void print(std::string _msg)
	{
		std::cout << _msg.c_str() << std::endl;
	}

	//std::wstring s2ws(const std::string& s)
	//{
	//	int len;
	//	int slength = (int)s.length() + 1;
	//	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	//	std::wstring r(len, L'\0');
	//	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, &r[0], len);
	//	return r;
	//}

	//std::string ws2s(const std::wstring& s)
	//{
	//	int len;
	//	int slength = (int)s.length() + 1;
	//	len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
	//	std::string r(len, '\0');
	//	WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, &r[0], len, 0, 0);
	//	return r;
	//}
}