#include <iostream>
#include "PoolAllocator.hpp"

PoolAllocator::PoolAllocator(size_t objectSize, size_t objectCount, size_t alignment)
    : objectSize(objectSize),
    objectCount(objectCount),
    alignment(alignment),
    rawMemory(nullptr),
    memoryBlock(nullptr),
    freeListHead(nullptr)
{

    this->objectSize = (objectSize + (alignment - 1)) & ~(alignment - 1);

    if (this->objectSize < sizeof(void*)) {
        this->objectSize = sizeof(void*);
    }
    
    size_t totalSize = this->objectSize * objectCount;

    rawMemory = std::malloc(totalSize + alignment);
    if (!rawMemory) {
        throw std::bad_alloc();
    }

    std::uintptr_t rawAddr = reinterpret_cast<std::uintptr_t>(rawMemory);
    std::uintptr_t alignedAddr = (rawAddr + (alignment - 1)) & ~(alignment - 1);
    memoryBlock = reinterpret_cast<void*>(alignedAddr);

    char* current = static_cast<char*>(memoryBlock);

    for (size_t i = 0; i < objectCount - 1; ++i)
    {
        void* next = current + this->objectSize;
        *reinterpret_cast<void**>(current) = next;
        current += this->objectSize;
    }

    *reinterpret_cast<void**>(current) = nullptr;
    freeListHead = memoryBlock;
}

PoolAllocator::~PoolAllocator()
{
    std::free(rawMemory);
}

void* PoolAllocator::Allocate()
{
    if (freeListHead == nullptr)
        return nullptr;

    void* allocated = freeListHead;

    freeListHead = *reinterpret_cast<void**>(freeListHead);

    return allocated;
}

void PoolAllocator::Free(void* ptr)
{
    if (!ptr)
        return;

    *reinterpret_cast<void**>(ptr) = freeListHead;
    freeListHead = ptr;
}