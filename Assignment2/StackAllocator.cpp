#include <iostream>
#include "StackAllocator.hpp"

StackAllocator::StackAllocator(size_t capacity)
    :m_capacity(capacity)
{
    m_base = static_cast<std::uint8_t*>(std::malloc(m_capacity));
    m_offset = 0;
}

StackAllocator::~StackAllocator()
{
    std::free(m_base);
    m_base = nullptr;
    m_capacity = 0;
    m_offset = 0;
}

void* Allocate(size_t size, size_t alignment){

}