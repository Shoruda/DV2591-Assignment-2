#include "Memory.hpp"
#include "PoolAllocator.hpp"
#include "StackAllocator.hpp"
#include "BuddyAllocator.hpp"
#include <chrono>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>



struct ObjectSmall{
	std::uint8_t data[32];
};
struct ObjectMedium{
	std::uint8_t data[128];
};
struct ObjectBig{
	std::uint8_t data[512];
};

enum class AllocMode {
	OS,
	Pool,
	Stack,
	Buddy,
	Stomp
};

template <typename T>
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
				ptrs[j] = malloc(sizeof(T));
				if (!ptrs[j]) {
					std::cout << "Out of memory!\n";
					break;
				}
				T obj;
				obj.data[0] = 69;
				std::memcpy(ptrs[j], &obj, sizeof(T));
			}
			for (int j = 0; j < 10; j++) {
				free(ptrs[j]);
			}		
		}
	}
	
	else if (mode == AllocMode::Pool) 
	{
		std::cout << "Running test with Pool allocator\n";
		PoolAllocator Pool(sizeof(T), 20, 16);
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
				T obj;
				obj.data[0] = 69;
				std::memcpy(ptrs[j], &obj, sizeof(T));
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
		const size_t capacity = sizeof(T) * allocsPerFrame;

		StackAllocator stack(capacity);

		for(int i = 0; i < frames; i++){
			stack.Reset();

			for(int j = 0; j < allocsPerFrame; j++){
				void* mem = stack.Allocate(sizeof(T), alignof(T));
				if(!mem)
					break;
				
				auto* obj = new(mem) T;
				obj->data[0] = 40;
			}
		}
	}

	else if (mode == AllocMode::Buddy)
	{
		std::cout << "Running test with Buddy allocator\n";
		BuddyAllocator Buddy(32, 128);

		void* ptr = Buddy.Allocate(sizeof(T));
		T obj;
		obj.data[0] = 67;
		std::memcpy(ptr, &obj, sizeof(T));
		Buddy.Deallocate(ptr);

		/*for (int i = 0; i < objectCount; i += 10)
		{
			void* ptrs[10];
			for (int j = 0; j < 10; j++)
			{
				ptrs[j] = Buddy.allocate(sizeof(T));
				if (!ptrs[j]) {
					std::cout << "Block full!\n";
					break;
				}
				T obj;
				obj.data[0] = 69;
				std::memcpy(ptrs[j], &obj, sizeof(T));
			}
			for (int j = 0; j < 10; j++) {
				Buddy.deallocate(ptrs[j]);
			}
		}*/
	}

	else if (mode == AllocMode::Stomp)
	{
		std::cout << "Running test with Stomp allocator\n";
		//do magic
		//futti - toe
	}
	
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Test finished in " << duration.count() << " ms\n\n";
    return duration.count();
}

int main()
{
	const int objectCount = 10000000;

	runTestAllocation<ObjectSmall>(AllocMode::Buddy, objectCount);

	/*double osTime = runTestAllocation<ObjectSmall>(AllocMode::OS, objectCount);
	double poolTime = runTestAllocation<ObjectSmall>(AllocMode::Pool, objectCount);
	double stackTime = runTestAllocation<ObjectSmall>(AllocMode::Stack, objectCount);

    std::cout << "Summary:\n";
    std::cout << "  OS allocator time:   " << osTime   << " ms\n";
    std::cout << "  Pool allocator time: " << poolTime << " ms\n";
	std::cout << "  Stack allocator time: " << stackTime << " ms\n";*/

    return 0;
}