#pragma once
#include "../Manager.h"

void (WINAPI *ExitOri)(UINT uExitCode) = ExitProcess;
void WINAPI ExitNew(UINT uExitCode)
{
	system("pause");
}

void (WINAPI *OriSysTime)(_Out_ LPSYSTEMTIME lpSystemTime) = GetSystemTime;
void WINAPI SysTime(_Out_ LPSYSTEMTIME lpSystemTime)
{
	lpSystemTime->wDay = 24;
	lpSystemTime->wDayOfWeek = 0;
	lpSystemTime->wHour = 0;
	lpSystemTime->wMilliseconds = 0;
	lpSystemTime->wMinute = 0;
	lpSystemTime->wMonth = 5;
	lpSystemTime->wSecond = 0;
	lpSystemTime->wYear = 2013;
}

bool isPause = false;
HANDLE(WINAPI *CrFileA)(
	_In_ LPCSTR lpFileName,
	_In_ DWORD dwDesiredAccess,
	_In_ DWORD dwShareMode,
	_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	_In_ DWORD dwCreationDisposition,
	_In_ DWORD dwFlagsAndAttributes,
	_In_opt_ HANDLE hTemplateFile) = CreateFileA;

HANDLE WINAPI NewCrFileA(
	_In_ LPCSTR lpFileName,
	_In_ DWORD dwDesiredAccess,
	_In_ DWORD dwShareMode,
	_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	_In_ DWORD dwCreationDisposition,
	_In_ DWORD dwFlagsAndAttributes,
	_In_opt_ HANDLE hTemplateFile)
{
#if PRINT_FILES
	cout << lpFileName << endl;
#endif
	if (!isPause)
	{
		Modules::Stages::CrFileA();
#if PAUSE
		system("pause");
#endif
		isPause = true;
	}
	return CrFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

bool isInit = false;
HWND(WINAPI *Window)(
	_In_ DWORD dwExStyle,
	_In_opt_ LPCWSTR lpClassName,
	_In_opt_ LPCWSTR lpWindowName,
	_In_ DWORD dwStyle,
	_In_ int X,
	_In_ int Y,
	_In_ int nWidth,
	_In_ int nHeight,
	_In_opt_ HWND hWndParent,
	_In_opt_ HMENU hMenu,
	_In_opt_ HINSTANCE hInstance,
	_In_opt_ LPVOID lpParam) = CreateWindowExW;

HWND WINAPI NewWindow(
	_In_ DWORD dwExStyle,
	_In_opt_ LPCWSTR lpClassName,
	_In_opt_ LPCWSTR lpWindowName,
	_In_ DWORD dwStyle,
	_In_ int X,
	_In_ int Y,
	_In_ int nWidth,
	_In_ int nHeight,
	_In_opt_ HWND hWndParent,
	_In_opt_ HMENU hMenu,
	_In_opt_ HINSTANCE hInstance,
	_In_opt_ LPVOID lpParam)
{
	auto hwnd = Window(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	if (!isInit)
	{
#if REPLACE_XDB
		HANDLE hThread = StartRoutine(Wave2);
		WaitForSingleObject(hThread, INFINITE);
#endif
		Modules::Client::SetMainWindow(hwnd);
		Modules::Stages::CrWindow();
		isInit = !isInit;
	}
	return hwnd;
}