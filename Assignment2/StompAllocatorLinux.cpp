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


}

void StompAllocatorLinux::deallocate(void* ptr)
{

	
}

bool StompAllocatorLinux::accessViolation(void(*func)())
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
