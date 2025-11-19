#pragma once
#include <iostream>
#include <vector>

class BuddyAllocator
{
public:
	BuddyAllocator(size_t minBlockSize, size_t totalSize);
	~BuddyAllocator();

	void* allocate(size_t size);
	void deallocate(void* ptr);

private:
	size_t m_minBlockSize;	//Smallest allowed block
	size_t m_totalSize;		//total size of all blocks
	void* m_basePtr;		//raw memory block

	std::vector<std::vector<void*>> m_freeblocks;
	std::vector<int> m_blocklevel;

	int getLevelForSize(size_t size);
	size_t getBlockSizeForLevel(size_t level);
	void split(int level);
	void merge(int level, int offset);
};
