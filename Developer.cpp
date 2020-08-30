#include "Manager.h"

void ShowConsole()
{
	if (AllocConsole()) {
		freopen("CONOUT$", "w", stdout);
		SetConsoleTitleA("Console");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		printf("Dll Succefull Attached!\n");
		printf("Debug Console Started...\n");
	}
}

void Initialization()
{
	Manager();
}

BOOL WINAPI DllMain(HMODULE hDll, DWORD dwReason, LPVOID)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
#if SHOW_CONSOLE
		ShowConsole();
#endif
		EnableFilter();
		StartRoutine(Initialization);
	}
	return TRUE;
}

extern "C" __declspec(dllexport) char * __stdcall InjectMe()
{
	return "Thank you! So Much!";
}