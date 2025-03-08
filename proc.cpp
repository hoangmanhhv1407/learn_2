//---------------------------------------------------------------------------

#pragma hdrstop

#include "proc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#include <tchar.h>
#include <TlHelp32.h>


DWORD Proc::GetProcId(const wchar_t* procName)
{
	//Take a snapshot of all processes and store it on a Handle
	const HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnap == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	PROCESSENTRY32 procEntry;
	procEntry.dwSize = sizeof(procEntry);

	if (!Process32First(hSnap, &procEntry))
	{
		//Unsuccessful attempt to retrieve information from process
		CloseHandle(hSnap);
		return 0;
	}

	do
	{
		if (_wcsicmp(procEntry.szExeFile, procName) == 0)
		{
			const DWORD procId = procEntry.th32ProcessID;
			CloseHandle(hSnap);
			return procId;
		}
	} while (Process32Next(hSnap, &procEntry));

	CloseHandle(hSnap);
	return 0;
}

uintptr_t Proc::GetModuleBaseAddress(DWORD procId, const wchar_t* modeNames)
{
	const HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (hModuleSnap == INVALID_HANDLE_VALUE)
		return 0;

	MODULEENTRY32 moduleEntry;
	moduleEntry.dwSize = sizeof(moduleEntry);

	if (!Module32First(hModuleSnap, &moduleEntry))
	{
		CloseHandle(hModuleSnap);
		return 0;
	}

	do
	{
		if (_wcsicmp(moduleEntry.szModule, modeNames) == 0)
		{
			const auto baseAddress = uintptr_t(moduleEntry.modBaseAddr);
			CloseHandle(hModuleSnap);
			return baseAddress;
		}
	} while (Module32Next(hModuleSnap, &moduleEntry));

	CloseHandle(hModuleSnap);
	return 0;
}

uintptr_t Proc::FindDynamicMemoryAddress(HANDLE hProc, uintptr_t ptr, std::vector<unsigned> offsets, ExecutionMode mode)
{
	return mode == ExecutionMode::External ? FindDynamicMemoryAddressEx(hProc, ptr, offsets) : FindDynamicMemoryAddressIn(hProc, ptr, offsets);
}

uintptr_t Proc::FindDynamicMemoryAddressEx(HANDLE hProc, uintptr_t ptr, std::vector<unsigned> offsets)
{
	uintptr_t address = ptr;
	for (auto offset : offsets)
	{
		ReadProcessMemory(hProc, (BYTE*)address, &address, sizeof(address), 0);
		address += offset;
	}

	return address;
}

uintptr_t Proc::FindDynamicMemoryAddressIn(HANDLE hProc, uintptr_t ptr, std::vector<unsigned> offsets)
{
	uintptr_t address = ptr;
	for (unsigned int offset : offsets)
	{
		address = *(uintptr_t*)address;
		address += offset;
	}
	return address;
}
