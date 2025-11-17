#include <iostream>
#include <cstdint>
#include <windows.h>
#include "Memory.hpp"
#include "PoolAllocator.hpp"

struct Object{
	std::uint8_t data[32];
};

int main()
{
	//PoolAllocator pool(sizeof(Object), 100);
	CustomAlloc(sizeof(Object), false);
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