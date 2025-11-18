#include "Memory.hpp"
#include "PoolAllocator.hpp"
#include "StackAllocator.hpp"
#include <iostream>

void* CustomAlloc(size_t size, size_t objectCount, bool custom) 
{ 
	if (custom)
	{
		//do stuff :) do magic !!!!
		return 0;
	}
		
	std::cout << "OS memory chom" << std::endl;
	return ::operator new(size);
	

}

void  CustomFree(void* ptr, bool custom) 
{ 
	if (custom)
	{
		//do stuff :)
	}
	else
	{
		free(ptr);
	}
}