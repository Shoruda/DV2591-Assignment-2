#include "Memory.hpp"
#include "PoolAllocator.hpp"
#include "StackAllocator.hpp"
#include "BuddyAllocator.hpp"
#include <chrono>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>



struct Object1{
	std::uint8_t data[32];
};
struct Object2{
	std::uint8_t data[128];
};
struct Object3{
	std::uint8_t data[512];
};

enum class AllocMode {
	OS,
	Pool,
	Stack,
	Buddy
};

double runTestAllocation(AllocMode mode, int objectCount){

	auto start = std::chrono::high_resolution_clock::now();

	if (mode == AllocMode::OS) 
	{
		std::cout << "Running test with OS allocator\n";
		for (int i = 0; i < objectCount; i += 10)
		{
			void* ptrs[10];
			for (int j = 0; j < 10; j++)
			{
				ptrs[j] = malloc(sizeof(Object1));
				if (!ptrs[j]) {
					std::cout << "Out of memory!\n";
					break;
				}
				Object1 obj;
				obj.data[0] = 69;
				std::memcpy(ptrs[j], &obj, sizeof(Object1));
			}
			for (int j = 0; j < 10; j++) {
				free(ptrs[j]);
			}		
		}
	}
	
	else if (mode == AllocMode::Pool) 
	{
		std::cout << "Running test with Pool allocator\n";
		PoolAllocator Pool(sizeof(Object1), 20, 16);
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
				Object1 obj;
				obj.data[0] = 69;
				std::memcpy(ptrs[j], &obj, sizeof(Object1));
			}
			for (int j = 0; j < 10; j++) {
				Pool.Free(ptrs[j]);
			}
		}
	}
	
	else if (mode == AllocMode::Stack) 
	{
		std::cout << "Running test with Stack allocator\n";
		const int frames = 10;
		const int allocsPerFrame = objectCount / frames;
		const size_t capacity = sizeof(Object1) * allocsPerFrame;

		StackAllocator stack(capacity);

		for(int i = 0; i < frames; i++){
			stack.Reset();

			for(int j = 0; j > allocsPerFrame; j++){
				void* mem = stack.Allocate(sizeof(Object1), alignof(Object1));
				if(!mem)
					break;
				
				auto* obj = new(mem) Object1;
				obj->data[0] = 40;
			}
		}
	}

	else if (mode == AllocMode::Buddy)
	{
		std::cout << "Running test with Buddy allocator\n";
		BuddyAllocator Buddy(32, 128);

		void* ptr = Buddy.allocate(sizeof(Object1));
		Object1 obj;
		obj.data[0] = 67;
		std::memcpy(ptr, &obj, sizeof(Object1));

		/*for (int i = 0; i < objectCount; i += 10)
		{
			void* ptrs[10];
			for (int j = 0; j < 10; j++)
			{
				ptrs[j] = Buddy.allocate(sizeof(Object1));
				if (!ptrs[j]) {
					std::cout << "Block full!\n";
					break;
				}
				Object1 obj;
				obj.data[0] = 69;
				std::memcpy(ptrs[j], &obj, sizeof(Object1));
			}
			for (int j = 0; j < 10; j++) {
				Buddy.deallocate(ptrs[j]);
			}
		}*/
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
	double stackTime = runTestAllocation(AllocMode::Stack, objectCount);

    std::cout << "Summary:\n";
    std::cout << "  OS allocator time:   " << osTime   << " ms\n";
    std::cout << "  Pool allocator time: " << poolTime << " ms\n";
	std::cout << "  Stack allocator time: " << stackTime << " ms\n";

    return 0;
}