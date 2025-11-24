#include "StompAllocator.hpp"

//#define StompDebug
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
	size_t total_pages = 1 + required_pages + 1; //header page + user page + guard page
	size_t total_bytes = m_pageSize * total_pages;

	void* base = VirtualAlloc(nullptr, total_bytes, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	if (!base)
	{
		std::cout << "ERROR: Stomp allocation : virtual alloc fail" << std::endl;
		return nullptr;
	}


	DWORD oldprotect = 0;

	char* header_page = (char*)base;
	char* user_ptr = header_page + m_pageSize;
	void* guard_page = (char*)base + (total_pages-1)*m_pageSize; // allign the guard pointer to the end




	#ifdef StompDebug
	std::cout << "base:        " << base << "\n";
	std::cout << "user_ptr:    " << (void*)user_ptr << "\n";
	std::cout << "guard_page:  " << guard_page << "\n";
	std::cout << "total_bytes: " << total_bytes << "\n";
	std::cout << "required_pages: " << required_pages << "\n";
	std::cout << "total_pages: " << total_pages << "\n";
	#endif

	
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

	if (!VirtualProtect(header_page, m_pageSize, PAGE_READONLY, &oldprotect))
	{
		std::cout << "ERROR: Stomp allocation : protecting header page" << std::endl;
		VirtualFree(base, 0, MEM_RELEASE);
		return nullptr;
	}

	
	return user_ptr;
}

void StompAllocator::deallocate(void* ptr)
{
	if (!ptr)
	{
		std::cout << "ERROR: stomp allocator: deallocate ptr not found\n";
	}
	else
	{
		char* header_page = (char*)ptr - m_pageSize;

		AllocationInfo* header = (AllocationInfo*)header_page;

		VirtualFree(header->baseAddress, 0, MEM_RELEASE);
		
	}
	
}


