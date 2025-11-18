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

int BuddyAllocator::getLevelForSize(size_t size)
{
	int level = 0;
	while (size > m_minBlockSize)
	{
		level += 1;
	}
	return level;
}

size_t BuddyAllocator::getBlockSizeForLevel(size_t level)
{
	size_t size = m_minBlockSize;
	for (int i = 0; i < level; i++)
	{
		size * 2;
	}
	return size;
}

void BuddyAllocator::split(int level)
{
}
