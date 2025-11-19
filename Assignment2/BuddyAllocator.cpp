#include "BuddyAllocator.hpp"

BuddyAllocator::BuddyAllocator(size_t minBlockSize, size_t totalSize)
	:m_minBlockSize(minBlockSize), m_totalSize(totalSize)
{
	m_basePtr = malloc(totalSize);
	int maxLevel = getLevelForSize(totalSize);
	m_freeblocks.resize(maxLevel + 1);
	m_freeblocks[maxLevel].push_back(m_basePtr);
}

BuddyAllocator::~BuddyAllocator()
{
	free(m_basePtr);
}

void* BuddyAllocator::allocate(size_t size)
{
	if (size > m_totalSize) return nullptr;

	int level = getLevelForSize(size);
	split(level);
	if (m_freeblocks[level].empty())
	{
		//no more memory to give :(
		return nullptr;
	}
	void* blkptr = m_freeblocks[level].back();
	m_freeblocks[level].pop_back();

	//Get the level needed for allocation size
	//Run split
	//Check if still empty (no memory left)
	//if not empty pop and return pointer

	return blkptr;
}

void BuddyAllocator::deallocate(void* ptr)
{
}

int BuddyAllocator::getLevelForSize(size_t size)
{
	int level = 0;
	int blocksize = getBlockSizeForLevel(level);
	while (size > blocksize)
	{
		level += 1;
		blocksize = getBlockSizeForLevel(level);
	}
	return level;
}

size_t BuddyAllocator::getBlockSizeForLevel(size_t level)
{
	size_t size = m_minBlockSize;
	for (int i = 0; i < level; i++)
	{
		size = size * 2;
	}
	return size;
}

void BuddyAllocator::split(int level)
{
	if (!m_freeblocks[level].empty())
	{
		return;
	}

	int maxlevel = m_freeblocks.size() - 1;
	if (level >= maxlevel)
	{
		//out of memory
		return;
	}

	split(level + 1);

	if (m_freeblocks[level + 1].empty())
	{
		//cannot split parent is also out of memory
		return;
	}

	void* parentBlk = m_freeblocks[level + 1].back();
	m_freeblocks[level + 1].pop_back();
	size_t halfPoint = getBlockSizeForLevel(level);

	char* child1 = (char*)parentBlk;
	char* child2 = child1 + halfPoint;

	m_freeblocks[level].push_back(child1);
	m_freeblocks[level].push_back(child2);

	//check if free block exists on level (if yes return, if no continue)
	//If we are at highest level no more memory
	//recurssion on level + 1
	//pop one parent block
	//add 2 children blocks
}
