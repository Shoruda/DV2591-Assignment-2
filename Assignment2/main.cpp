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

		for(int i = 0; i < objectCount; i+=10)
		{
			std::vector<void*> pointers;
			for (int j = 0; j < 10; j++)
			{
				void* ptr = nullptr;
				ptr = malloc(sizeof(Object));
				pointers.push_back(ptr);
			}
			for (void* ptr : pointers) {
				free(ptr);
			}
		}
	}
	else if (mode == AllocMode::Pool) {
		std::cout << "Running test with Pool allocator\n";

		PoolAllocator Pool(sizeof(Object), 20);

		for (int i = 0; i < objectCount; i+=10) 
		{
			std::vector<void*> pointers;
			for (int j = 0; j < 10; j++)
			{
				void* raw = Pool.Allocate();
				if (!raw) {
					std::cout << "Pool exhausted!\n";
					break;
				}
				pointers.push_back(raw);
			}
			for (void* ptr : pointers) {
				Pool.Free(ptr);
			}
		}
	}
	
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Test finished in " << duration.count() << " ms\n\n";
    return duration.count();
}

int main()
{
	const int objectCount = 100000;

	double osTime = runTestAllocation(AllocMode::OS, objectCount);
	double poolTime = runTestAllocation(AllocMode::Pool, objectCount);

    std::cout << "Summary:\n";
    std::cout << "  OS allocator time:   " << osTime   << " ms\n";
    std::cout << "  Pool allocator time: " << poolTime << " ms\n";

    return 0;
}