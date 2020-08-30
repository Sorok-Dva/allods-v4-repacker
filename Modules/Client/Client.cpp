#include "../Modules.h"

int version = 8455;
bool isVersion = false;

HWND mainWindow = NULL;

void patchVersion()
{
	ASM(a);
	a.mov(eax, version);
	a.add(esp, 0x8);
	a.cmp(byte_ptr(ebp, -0x1), 0x0);
	a.jmp(0x0062BFEB);
	patch.PlaceJit(0x0062BFE4, &a, 2);
	isVersion = true;
}

void Modules::Client::SetMainWindow(HWND hwnd)
{
	mainWindow = hwnd;
}

HWND Modules::Client::GetMainWindow()
{
	return mainWindow;
}

void Modules::Client::SetVersion(int v)
{
	if (!isVersion)
		patchVersion();
	version = v;
}

void Modules::Client::DisableCheckFiles()
{
	X86Assembler a(&runtime);
	a.jmp(0x00F69DA5);
	patch.PlaceCode(0x00F69D64, &a);
}

void Modules::Client::EnableProtectedLua()
{
	patch.SetNOPS(0x005B0539, 2);
	patch.SetNOPS(0x004E5477, 2);
}

void Modules::Client::DisableAbort()
{
	ASM(a);
	a.ret();
	patch.PlaceJit(GetProcAddress(GetModuleHandle("MSVCR100.dll"), "abort"), &a);
}
