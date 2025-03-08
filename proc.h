//---------------------------------------------------------------------------

#ifndef procH
#define procH
//---------------------------------------------------------------------------
#endif
#pragma once
#include <Windows.h>
#include <vector>

class Proc
{
public:
	enum class ExecutionMode
	{
		External,
		Internal
	};
public:
	static DWORD GetProcId(const wchar_t* procName);

	static uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modeNames);

	static uintptr_t FindDynamicMemoryAddress(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets, ExecutionMode mode = ExecutionMode::External);

	static uintptr_t FindDynamicMemoryAddressEx(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);

	static uintptr_t FindDynamicMemoryAddressIn(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);
	Proc();
	~Proc();
};

