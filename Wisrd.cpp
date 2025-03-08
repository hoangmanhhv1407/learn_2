//---------------------------------------------------------------------------

#pragma hdrstop

#include "Wisrd.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#include <Windows.h>
#include <vector>
#include <TlHelp32.h>

Wirsd::Wirsd(std::string game)
{
	m_game = game;
	grabHandle();
}
Wirsd::~Wirsd()
{
	CloseHandle(m_handle);
}
DWORD Wirsd::GetProcId3(const wchar_t* procName)
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
HANDLE Wirsd::grabHandle()
{
	HWND gameWindow;
	DWORD procID;
    const wchar_t* namegame = L"NINEDRAGONS_Completed.exe";
	gameWindow = FindWindowA(NULL, m_game.c_str());
    procID = GetProcId3(namegame);
	//GetWindowThreadProcessId(gameWindow, &procID);
	m_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);
	return m_handle;
}