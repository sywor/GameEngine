#pragma once
#include <iostream>
#include <limits>

namespace utilities
{
	static const float pi = 3.14159265359f;

	void pause()
	{
		std::cout << "Press \"the\" any key twice to close the program" << std::endl;
		std::cin.get();		
	}
}