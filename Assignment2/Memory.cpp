#include "Memory.hpp"
#include "PoolAllocator.hpp"
#include "StackAllocator.hpp"
#include <iostream>

void* CustomAlloc(size_t size, bool custom) 
{ 
	if (custom)
	{
		//do stuff :)
		return 0;
	}
	else
	{
		return malloc(size);
	}
}

void  CustomFree(void* ptr) 
{ 
	
}