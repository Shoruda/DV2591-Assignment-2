#pragma once

#include <cstddef>

void* CustomAlloc(size_t size, size_t objectCount, bool custom);
void  CustomFree(void* ptr);