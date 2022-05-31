#pragma once

#include <wdm.h>

#define ALLOC_MEMORY(_size) ExAllocatePool2(POOL_FLAG_NON_PAGED, _size, 'YNIT')
#define FREE_MEMORY(__mem) ExFreePoolWithTag(__mem, 0)

namespace tiny {
template <typename T>
inline void global_object_pointer_initialize(T** globalObjectPointer)
{
	*globalObjectPointer = reinterpret_cast<T*>(ALLOC_MEMORY(sizeof(T)));
	**globalObjectPointer = T();
}

template <typename T>
inline void global_object_pointer_destroy(T** globalObjectPointer)
{
	(*globalObjectPointer)->~T();
	FREE_MEMORY(*globalObjectPointer);
	*globalObjectPointer = nullptr;
}
}