#pragma once

#include <ntifs.h>

#define ALLOC_MEMORY(_size) ExAllocatePool2(POOL_FLAG_NON_PAGED, _size, 'YNIT')
#define FREE_MEMORY(__mem) ExFreePoolWithTag(__mem, 0)

void* __cdecl operator new(size_t Size) noexcept(false);
void __cdecl operator delete(void* mem);

namespace tiny {
	template <typename T>
	inline void global_object_pointer_initialize(T** globalObjectPointer)
	{
		*globalObjectPointer = new T();
	}

	template <typename T>
	inline void global_object_pointer_destroy(T** globalObjectPointer)
	{
		delete *globalObjectPointer;
		*globalObjectPointer = nullptr;
	}
}