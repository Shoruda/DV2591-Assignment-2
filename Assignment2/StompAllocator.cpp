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

	if (!base)
	{
		std::cout << "ERROR: Stomp allocation : virtual alloc fail" << std::endl;
		return nullptr;
	}


	DWORD oldprotect = 0;

	char* header_page = (char*)base;
	char* user_ptr = header_page + m_pageSize;
	void* guard_page = (char*)base + required_pages * m_pageSize; // allign the guard pointer to the end

	if (!VirtualProtect(guard_page, m_pageSize, PAGE_NOACCESS, &oldprotect))
	{
		std::cout << "ERROR: Stomp allocation : guard allocation" << std::endl;
		VirtualFree(base, 0, MEM_RELEASE);
		return nullptr;
	}


	AllocationInfo* header = (AllocationInfo*)base;
	header->baseAddress = base;
	header->allocated_pages = total_pages;
	header->requested_size = size;
	
	
	return user_ptr;
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

bool StompAllocator::accessViolation(void(*func)())
{
	_try{
		func();
	}
	_except(GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION 
			? EXCEPTION_EXECUTE_HANDLER 
			: EXCEPTION_CONTINUE_SEARCH)
	{
		return true;
	}
	return false;
}
