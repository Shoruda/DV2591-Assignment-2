#include "StompAllocatorLinux.hpp"


//#define StompDebug

StompAllocatorLinux::StompAllocatorLinux()
{
}

StompAllocatorLinux::~StompAllocatorLinux()
{
}

void* StompAllocatorLinux::allocate(size_t size)
{

	size_t required_pages = (size + m_pageSize - 1) / m_pageSize; //roof division
	size_t total_pages = 1 + required_pages + 1; //header page + user page + guard page
	size_t total_bytes = m_pageSize * total_pages;



	//guard protect


	


	return nullptr;
}

void StompAllocatorLinux::deallocate(void* ptr)
{

	
}


