#pragma once
#include <iostream>
#include <vector>
#include <Windows.h>


class StompAllocator
{
public:
	StompAllocator();
	~StompAllocator();

	void* allocate(size_t size);
	void deallocate(void* ptr);
	bool accessViolation(void(*func)());

private:
	size_t m_pageSize;

	struct AllocationInfo
	{
		size_t requested_size;
		size_t allocated_pages;
		void* baseAddress;   // The base returned by VirtualAlloc
	};

};
