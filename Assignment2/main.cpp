#include <iostream>
#include <windows.h>
#include "Memory.hpp"
#include "PoolAllocator.hpp"

int main()
{
	std::cout << "Hello World!" << std::endl;
	while (true)
	{

		std::cout << "LOOPIN" << std::endl;
		if (GetKeyState(VK_ESCAPE) & 0x8000)
		{
			break;
		}
	}
	return 0;
}
//herro