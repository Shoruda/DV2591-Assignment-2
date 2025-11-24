#include "Memory.hpp"
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

bool accessViolation(const char* name, void(*func)())
{
	std::cout << "Test: ";
	std::cout << name;
	_try{
		func();
	}
	_except(EXCEPTION_EXECUTE_HANDLER)
	{
		
		std::cout << " :Returned an access violation\n";
		return true;
	}
	std::cout << " :Did not return a access violation\n";
	return false;
}

void testOverrun()
{
	char* ptr =(char*)StompAlloc(40);
	ptr[4096] = 0xAB;
	// Should give a write access violation 
}

void testUnderrun()
{
	char* ptr = (char*)StompAlloc(40);
	ptr[-1] = 0xAB;
	//Should give a write access violation
}

void testUseAfterFree() 
{
	char* ptr = (char*)StompAlloc(40);
	StompDeAlloc(ptr);

	ptr[0] = 0xAB;
	//Should give access violation
}



void StompAllocationTests()
{
	InitStomp();
	//accessViolation("Overrun", testOverrun);
	
	//accessViolation("Underrun", testUnderrun);
	accessViolation("Use after free", testUseAfterFree);
}


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
		InitPool(sizeof(T), 20, 16);
		for (int i = 0; i < objectCount; i += 10)
		{
			void* ptrs[10];
			for (int j = 0; j < 10; j++)
			{
				ptrs[j] = PoolAlloc();
				if (!ptrs[j]) {
					std::cout << "Pool exhausted!\n";
					break;
				}
				T obj;
				obj.data[0] = 69; // futti plus sjunio
				std::memcpy(ptrs[j], &obj, sizeof(T));
			}
			for (int j = 0; j < 10; j++) {
				PoolFree(ptrs[j]);
			}
		}
	}
	
	else if (mode == AllocMode::Stack) 
	{
		std::cout << "Running test with Stack allocator\n";
		const int frames = 10;
		const int allocsPerFrame = objectCount / frames;
		const size_t capacity = sizeof(T) * allocsPerFrame;

		InitStack(capacity);

		for(int i = 0; i < frames; i++){
			StackReset();

			for(int j = 0; j < allocsPerFrame; j++){
				void* ptrs = StackAlloc(sizeof(T), alignof(T));
				if(!ptrs)
					break;
				
				T obj;
				obj.data[0] = 69;
				std::memcpy(ptrs, &obj, sizeof(T));
			}
		}
	}

	else if (mode == AllocMode::Buddy)
	{
		std::cout << "Running test with Buddy allocator\n";
		InitBuddy(32, 1024);

		for (int i = 0; i < objectCount; i += 10)
		{
			void* ptrs[10];
			for (int j = 0; j < 10; j++)
			{
				ptrs[j] = BuddyAlloc(sizeof(T));
				if (!ptrs[j]) {
					std::cout << "Block full!\n";
					break;
				}
				T obj;
				obj.data[0] = 69;
				std::memcpy(ptrs[j], &obj, sizeof(T));
			}
			for (int j = 0; j < 10; j++) {
				BuddyDeAlloc(ptrs[j]);
			}
		}
	}

	else if (mode == AllocMode::Stomp)
	{
		std::cout << "Running test with Stomp allocator\n";
		InitStomp();
		for (int i = 0; i < objectCount; i+=10)
		{
			void* ptrs[10];
			for (int j = 0; j < 10; j++)
			{
				ptrs[j] = StompAlloc(sizeof(T));
				if (!ptrs[j]) {
					std::cout << "ERROR: stomp exhausted!\n";
					break;
				}
				T obj;
				obj.data[0] = 69;
				std::memcpy(ptrs[j], &obj, sizeof(T));
			}
			for (int j = 0; j < 10; j++)
			{
				StompDeAlloc(ptrs[j]);
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

	double osTime = runTestAllocation<ObjectSmall>(AllocMode::OS, objectCount);
	double poolTime = runTestAllocation<ObjectSmall>(AllocMode::Pool, objectCount);
	double stackTime = runTestAllocation<ObjectSmall>(AllocMode::Stack, objectCount);
	double buddyTime = runTestAllocation<ObjectSmall>(AllocMode::Buddy, objectCount);
	double stompTime = runTestAllocation<ObjectSmall>(AllocMode::Stomp, objectCount);

    std::cout << "Summary:\n";
    std::cout << "  OS allocator time:   " << osTime   << " ms\n";
    std::cout << "  Pool allocator time: " << poolTime << " ms\n";
	std::cout << "  Stack allocator time: " << stackTime << " ms\n";
	std::cout << "  Buddy allocator time: " << buddyTime << " ms\n";
	std::cout << "  Stomp allocator time: " << stompTime << " ms\n";

    return 0;
}