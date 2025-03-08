//---------------------------------------------------------------------------

#ifndef WisrdH
#define WisrdH
//---------------------------------------------------------------------------
#endif
#pragma once
#include <Windows.h>
#include <string>
class Wirsd {
private:
	HANDLE m_handle;
	std::string m_game;
public:
	Wirsd(std::string game);
	DWORD GetProcId3(const wchar_t* procName);
	~Wirsd();
	HANDLE grabHandle();

	template <class T>
	void write(DWORD addy, T value)
	{
		WriteProcessMemory(m_handle, (LPVOID)addy, &value, sizeof(T), NULL);
	}
	template <class T>
	void read(DWORD addy, T value)
	{
		ReadProcessMemory(m_handle, (LPVOID)addy, &value, sizeof(T), NULL);
	}
	template <class T>
	void writePointer(int numbeOffset, DWORD offset[], DWORD basse, T value)
	{
		DWORD templ;
		read<DWORD>(basse, templ);
		int i;
		for ( i = 0; i < numbeOffset-1; i++)
		{
			read<DWORD>(templ + offset[i], templ);
		}
		write<T>(templ + offset[i], value);
	}
	template <class T>
	void readPointer(int numbeOffset, DWORD offset[], DWORD basse, T& value)
	{
		DWORD templ;
		read<DWORD>(basse, templ);
		int i;
		for (i = 0; i < numbeOffset - 1; i++)
		{
			read<DWORD>(templ + offset[i], templ);
		}
		read<T>(templ + offset[i], value);
	}
};