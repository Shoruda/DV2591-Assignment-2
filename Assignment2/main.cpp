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

		if (mode == AllocMode::OS) {
			std::cout << "Running test with OS allocator\n";
			for (int i = 0; i < objectCount; i += 10)
			{
				void* ptrs[10];
				for (int j = 0; j < 10; j++)
				{
					ptrs[j] = malloc(sizeof(Object));
					Object obj;
					obj.data[0] = 69;
					memcpy(ptrs[j], &obj, sizeof(Object));
				}
				for (int j = 0; j < 10; j++) {
					free(ptrs[j]);
				}		
			}
		}
	}
	else if (mode == AllocMode::Pool) {
		std::cout << "Running test with Pool allocator\n";
		PoolAllocator Pool(sizeof(Object), 20);
		for (int i = 0; i < objectCount; i += 10)
		{
			void* ptrs[10];
			for (int j = 0; j < 10; j++)
			{
				ptrs[j] = Pool.Allocate();
				if (!ptrs[j]) {
					std::cout << "Pool exhausted!\n";
					break;
				}
				Object obj;
				obj.data[0] = 69;
				memcpy(ptrs[j], &obj, sizeof(Object));
			}
			for (int j = 0; j < 10; j++) {
				Pool.Free(ptrs[j]);
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
	const int objectCount = 10000000;

	double osTime = runTestAllocation(AllocMode::OS, objectCount);
	double poolTime = runTestAllocation(AllocMode::Pool, objectCount);

    std::cout << "Summary:\n";
    std::cout << "  OS allocator time:   " << osTime   << " ms\n";
    std::cout << "  Pool allocator time: " << poolTime << " ms\n";

    return 0;
}