#include "Memory.hpp"
#include "PoolAllocator.hpp"
#include <chrono>
#include <cstdint>
#include <iostream>
#include <vector>
#include <windows.h>


struct Object{
	std::uint8_t data[32];
};

enum class AllocMode {
	OS,
	Pool
};

double runTestAllocation(AllocMode mode, int objectCount){

	auto start = std::chrono::high_resolution_clock::now();

	if(mode == AllocMode::OS){
		std::cout << "Running test with OS allocator\n";

		std::vector<void*> pointers;

		for(int i = 0; i < objectCount; i++){
			void* ptr = nullptr;
			ptr = malloc(sizeof(Object));
			pointers.push_back(ptr);
		}
		for(void* ptr : pointers){
			delete ptr;
		}
	}
	else if(mode == AllocMode::Pool){

	}
	
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Test finished in " << duration.count() << " ms\n\n";
    return duration.count();
}

int main()
{
	const int objectCount = 10000;

	double osTime = runTestAllocation(AllocMode::OS, objectCount);
	//double poolTime = runTestAllocation(AllocMode::Pool, object);

    std::cout << "Summary:\n";
    std::cout << "  OS allocator time:   " << osTime   << " ms\n";
    //std::cout << "  Pool allocator time: " << poolTime << " ms\n";

    return 0;
}