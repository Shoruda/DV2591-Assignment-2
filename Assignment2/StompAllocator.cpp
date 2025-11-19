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

void* StompAllocator::allocate(size_t size, size_t alignment)
{

	size_t required_pages = (size + m_pageSize - 1) / m_pageSize; //roof division
	size_t total_pages = required_pages + 1; // guard page
	size_t total_bytes = m_pageSize * required_pages;

	void* base = VirtualAlloc(nullptr, total_bytes, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	DWORD oldprotect = 0;
	void* guard = (char*)base + required_pages * m_pageSize; // allign the guard pointer to the end
	if (!guard)
	{
		VirtualProtect(guard, m_pageSize, PAGE_NOACCESS, &oldprotect);
	}
	else
	{
		std::cout << "ERROR: Stomp allocation : guard allocation" << std::endl;
		return nullptr;
	}

	AllocationInfo* header = (AllocationInfo*)base;
	header->baseAddress = base;
	header->allocated_pages = total_pages;
	header->requested_size = size;
	
	void* usePtr = (char*)base + sizeof(AllocationInfo);
	
	return usePtr;
}

void StompAllocator::deallocate(void* ptr)
{
	auto header = (AllocationInfo*)((char*)ptr - sizeof(AllocationInfo));
	VirtualFree(header->baseAddress, 0, MEM_RELEASE);
}
