#include "StompAllocator.hpp"

StompAllocator::StompAllocator()
{
}

StompAllocator::~StompAllocator()
{
}

void* StompAllocator::allocate(size_t size, size_t alignment)
{
	return nullptr;
}

void StompAllocator::deallocate(void* ptr)
{
}
