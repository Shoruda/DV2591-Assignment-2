#pragma once
#include <cstddef>

void InitPool(size_t poolObjectSize, size_t poolObjectCount, size_t poolAlignment);
void InitStack(size_t stackSize);
void InitStomp();

void ShutdownMemory();

void* PoolAlloc();
void  PoolFree(void* ptr);

void* StackAlloc(size_t size, size_t alignment = 16);
void  StackReset();

void* StompAlloc(size_t size);
void StompDeAlloc(void* ptr);

