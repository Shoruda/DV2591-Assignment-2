#include <iostream>
#include <cstdint>
#include <windows.h>
#include "Timer.hpp"
#include "Memory.hpp"
#include "PoolAllocator.hpp"


struct Object{
	std::uint8_t data[32];
};

enum class AllocMode{
	OS,
	Pool
}

double runTestAllocation(AllocMode mode, int objectCount){
 	std::cout << "Running test with " 
		<< (mode == AllocMode::OS ? "OS allocator" : "Pool allocator") 
		<< " (" << objectCount << " objects)...\n";

	static PoolAllocator pool(sizeof(Object), 100000);

	std::vector<void*> pointers;
	pointers.reverse(objectCount);

	Timer timer;
	timer.start();

	if(mode == AllocMode::OS){
		for(int i = 0; i < objectCount; i++)
	}
	else if(mode == AllocMode.Pool){
		
	}
}

int main()
{
	const int objectCount = 10000;

	double osTime = runTestAllocation(AllocMode::OS, objectCount);
	//double poolTime = runTestAllocation(AllocMode::Pool, object);

    std::cout << "Summary:\n";
    std::cout << "  OS allocator time:   " << osTime   << " ms\n";
    std::cout << "  Pool allocator time: " << poolTime << " ms\n";

    return 0;
}