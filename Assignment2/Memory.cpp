#include "Memory.hpp"
#include "PoolAllocator.hpp"
#include "StackAllocator.hpp"
#include <iostream>

void* CustomAlloc(size_t size, bool custom) 
{ 
	if (custom)
	{
		return malloc(size);
	}
	else
	{
		return 0;
	}
}

void  CustomFree(void* ptr) 
{ 
	
}