#include <iostream>
#include <windows.h>

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