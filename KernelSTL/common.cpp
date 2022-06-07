#include "common.hpp"

void __cdecl operator delete(void*, unsigned __int64)
{
	/* It is required to define this operator in order to call destructor
	* inside global_object_pointer_destroy function.
	* This operator is not used but kernel requires it for proper linking.
	*/
}

void __cdecl operator delete(void* mem)
{
	FREE_MEMORY(mem);
}

void* __cdecl operator new(size_t Size) noexcept(false)
{
	void* memory = ALLOC_MEMORY(Size);
	if (!memory)
		ExRaiseStatus(STATUS_MEMORY_NOT_ALLOCATED);

	return memory;
}
