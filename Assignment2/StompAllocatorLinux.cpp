#include "StompAllocatorLinux.hpp"

#ifdef _WIN32

#else
#include <unistd.h>
#include <sys/mman.h>
#endif
//#define StompDebug

StompAllocatorLinux::StompAllocatorLinux()
{
	//m_pageSize = sysconf(_SC_PAGESIZE);
}

StompAllocatorLinux::~StompAllocatorLinux()
{
}

void* StompAllocatorLinux::allocate(size_t size)
{

	size_t required_pages = (size + m_pageSize - 1) / m_pageSize; //roof division
	size_t total_pages = 1 + required_pages + 1; //header page + user page + guard page
	size_t total_bytes = m_pageSize * total_pages;


	return nullptr;
}

void StompAllocatorLinux::deallocate(void* ptr)
{

	
}


