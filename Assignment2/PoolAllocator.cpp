#include <iostream>
#include "PoolAllocator.hpp"

PoolAllocator::PoolAllocator(size_t objectSize, size_t objectCount)
    : objectSize(objectSize),
    objectCount(objectCount),
    memoryBlock(nullptr),
    freeListHead(nullptr)
{
    if (objectSize < sizeof(void*)) {
        this->objectSize = sizeof(void*);
    }

    size_t totalSize = this->objectSize * objectCount;
    memoryBlock = std::malloc(totalSize);

    freeListHead = memoryBlock;

    char* current = static_cast<char*>(memoryBlock);

    for (size_t i = 0; i < objectCount - 1; ++i)
    {
        void* next = current + this->objectSize;
        *reinterpret_cast<void**>(current) = next;
        current += this->objectSize;
    }

    *reinterpret_cast<void**>(current) = nullptr;
}

PoolAllocator::~PoolAllocator()
{
    std::free(memoryBlock);
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