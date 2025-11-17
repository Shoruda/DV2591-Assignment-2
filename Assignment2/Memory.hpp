#pragma once

#include <cstddef>

void* CustomAlloc(size_t size, bool custom);
void  CustomFree(void* ptr, bool custom);
