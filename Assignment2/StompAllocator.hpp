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
};
