#pragma once
#include <iostream>
#include <vector>

class StompAllocator
{
public:
	StompAllocator(size_t minBlockSize, size_t totalSize);
	~StompAllocator();

	void* allocate(size_t size);
	void deallocate(void* ptr);

private:
	size_t m_minBlockSize;	//Smallest allowed block
	size_t m_totalSize;		//total size of all blocks
	void* m_basePtr;		//raw memory block
};
