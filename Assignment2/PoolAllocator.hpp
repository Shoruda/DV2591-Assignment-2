#pragma once

class PoolAllocator
{
public:
    PoolAllocator(size_t objectSize, size_t objectCount); //, size_t alignment?
    ~PoolAllocator();

    void* Allocate();
    void Free(void* ptr);
private:
    size_t objectSize;
    size_t objectCount;
    void* memoryBlock;
    void* freeListHead;
};