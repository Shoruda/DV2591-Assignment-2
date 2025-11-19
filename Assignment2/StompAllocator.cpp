#include "StompAllocator.hpp"



StompAllocator::StompAllocator()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	m_pageSize = si.dwPageSize;
}

StompAllocator::~StompAllocator()
{
}

void* StompAllocator::allocate(size_t size)
{

	size_t required_pages = (size + m_pageSize - 1) / m_pageSize; //roof division
	size_t total_pages = required_pages + 1; // guard page
	size_t total_bytes = m_pageSize * required_pages;

	void* base = VirtualAlloc(nullptr, total_bytes, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	DWORD oldprotect = 0;
	void* guard = (char*)base + required_pages *(1+ m_pageSize); // allign the guard pointer to the end
	if (!VirtualProtect(guard, m_pageSize, PAGE_NOACCESS, &oldprotect))
	{
		std::cout << "ERROR: Stomp allocation : guard allocation" << std::endl;
		return nullptr;
	}


	AllocationInfo* header = (AllocationInfo*)base;
	header->baseAddress = base;
	header->allocated_pages = total_pages;
	header->requested_size = size;
	
	char* usePtr = (char*)base + m_pageSize;
	
	return usePtr;
}

void StompAllocator::deallocate(void* ptr)
{
	if (!ptr)
	{
		std::cout << "ERROR: stomp allocator: deallocate ptr not found";
	}
	else
	{
		auto header = (AllocationInfo*)((char*)ptr - sizeof(AllocationInfo));
		VirtualFree(header->baseAddress, 0, MEM_RELEASE);
	}
	
}
