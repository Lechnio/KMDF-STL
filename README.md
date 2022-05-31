## Windows Kernel Tiny STL
Simple Windows Kernel STL containers implementation.

#### Usage
Copy all includes to your project and include `tiny_stl.hpp`.

#### Example
```cpp
#include <wdm.h>
#include "tiny_stl.hpp"

#define Message(msg, ...) do {DbgPrintEx(0, 0, "[Tiny]: " msg "\n", __VA_ARGS__);}while(0)

static tiny::vector<int>* vectorPtr;

extern "C" void DriverUnload(PDRIVER_OBJECT pDriverObject);
extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pUniStr)
{
	UNREFERENCED_PARAMETER(pUniStr);

	pDriverObject->DriverUnload = DriverUnload;

	tiny::global_object_pointer_initialize(&vectorPtr);

	vectorPtr->push_back(2);
	vectorPtr->push_back(1);
	vectorPtr->push_back(3);
	vectorPtr->push_back(7);

	Message("Driver loaded");
	return STATUS_SUCCESS;
}

extern "C" void DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	UNREFERENCED_PARAMETER(pDriverObject);

	Message("vectorPtr->size(): %zu", vectorPtr->size());
	for (const auto& v : *vectorPtr) 
		Message("vectorPtr: [%d]", v);

	tiny::global_object_pointer_destroy(&vectorPtr);

	Message("Driver unloaded");
}
```
Out from [DbgView](https://docs.microsoft.com/en-us/sysinternals/downloads/debugview):
```
00000001	0.00000000	[Tiny]: Driver loaded	
00000002	4.13198900	[Tiny]: vectorPtr->size(): 4	
00000003	4.13199139	[Tiny]: vectorPtr: [2]	
00000004	4.13199186	[Tiny]: vectorPtr: [1]	
00000005	4.13199234	[Tiny]: vectorPtr: [3]	
00000006	4.13199329	[Tiny]: vectorPtr: [7]	
00000007	4.13199377	[Tiny]: Driver unloaded	
```

#### Tests
Implemented tests guarantee `tiny::` containters behaviour to be comatible with `std::` containers.\
Running tests:
```cpp
#include "tests.hpp"

// some code ...
tiny::runTests();
// ...
```
