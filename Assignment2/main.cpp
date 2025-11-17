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

	Timer timer;
	timer.start();

	if(mode == AllocMode::OS){
		std::cout << "Running test with OS allocator\n";

		std::vector<void*> pointers;
		pointers.reverse(objectCount);

		for(int i = 0; i < objectCount; i++){
			void* ptr = nullptr;
			ptr = malloc(sizeof(Object))
			pointers.push_back(ptr);
		}
		for(void* ptr : pointers){
			delete ptr;
		}
	}
	else if(mode == AllocMode.Pool){

	}
	
	double ms = timer.elapsedMilliseconds();
    std::cout << "Test finished in " << ms << " ms\n\n";
    return ms;
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