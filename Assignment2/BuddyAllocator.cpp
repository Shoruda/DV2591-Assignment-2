#include "BuddyAllocator.hpp"

BuddyAllocator::BuddyAllocator(size_t minBlockSize, size_t totalSize)
	:m_minBlockSize(minBlockSize), m_totalSize(totalSize)
{
	m_basePtr = malloc(totalSize);
}

BuddyAllocator::~BuddyAllocator()
{
	free(m_basePtr);
}

void* BuddyAllocator::allocate(size_t size)
{
	return nullptr;
}

void BuddyAllocator::deallocate(void* ptr)
{
}
