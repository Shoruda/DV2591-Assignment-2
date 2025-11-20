#include "Memory.hpp"
#include "PoolAllocator.hpp"
#include "StackAllocator.hpp"
#include "StompAllocator.hpp"
#include <iostream>

static PoolAllocator* g_pool = nullptr;
static StackAllocator* g_stack = nullptr;
static StompAllocator* g_stomp = nullptr;

void InitPool(size_t poolObjectSize, size_t poolObjectCount, size_t poolAlignment)
{
    g_pool = new PoolAllocator(poolObjectSize, poolObjectCount, poolAlignment);
}

void InitStack(size_t stackSize)
{
    g_stack = new StackAllocator(stackSize);
}

void InitStomp()
{
    g_stomp = new StompAllocator();
}

void ShutdownMemory()
{
    delete g_pool;
    delete g_stack;
    delete g_stomp;
    g_pool = nullptr;
    g_stack = nullptr;
    g_stomp = nullptr;
}

void* PoolAlloc()
{
    if (!g_pool)
    {
        std::cout << "[Pool] ERROR: pool not initialized\n";
        return nullptr;
    }

    return g_pool->Allocate();
}

void PoolFree(void* ptr)
{
    if (!g_pool || !ptr) return;
    g_pool->Free(ptr);
}

void* StackAlloc(size_t size, size_t alignment)
{
    if (!g_stack)
    {
        std::cout << "[Stack] ERROR: stack not initialized\n";
        return nullptr;
    }

    return g_stack->Allocate(size, alignment);
}

void StackReset()
{
    if (g_stack)
        g_stack->Reset();
}

void* StompAlloc(size_t size)
{
    if (!g_stomp)
    {
        std::cout << "[STOMP] ERROR: stomp not initialized\n";
        return nullptr;
    }
    return g_stomp->allocate(size);
}

void StompDeAlloc(void* ptr)
{
    if (!g_stomp)
    {
        std::cout << "[STOMP] ERROR: stomp not initialized\n";
    }
    else
    {
        g_stomp->deallocate(ptr);
    }
}

