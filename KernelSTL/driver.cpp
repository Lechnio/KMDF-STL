//
// Kernel STL containers example
//

#include "tiny_stl.hpp"
#include "tests.hpp"

extern "C" void DriverUnload(PDRIVER_OBJECT pDriverObject);
extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pUniStr)
{
	UNREFERENCED_PARAMETER(pUniStr);

	pDriverObject->DriverUnload = DriverUnload;

	tiny::runTests();

	return STATUS_SUCCESS;
}

extern "C" void DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	UNREFERENCED_PARAMETER(pDriverObject);
}