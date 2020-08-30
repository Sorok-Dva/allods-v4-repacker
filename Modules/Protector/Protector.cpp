#include "../Modules.h"

void GetModuleInfo(char* moduleName, MODULEENTRY32* mInfo, bool aboutProcess = false)
{
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
	mInfo->dwSize = sizeof(MODULEENTRY32);

	if (Module32First(snapshot, mInfo))
	{
		if (aboutProcess) { CloseHandle(snapshot); return; }
		while (Module32Next(snapshot, mInfo))
		{
			if (_stricmp(moduleName, mInfo->szModule) == 0)
			{
				CloseHandle(snapshot);
				return;
			}
		}
	}
	CloseHandle(snapshot);
	memset(mInfo, 0, sizeof(MODULEENTRY32));
}
MODULEENTRY32 m;
LPVOID mirror = NULL;

void redirect()
{
	ASM(a);
	a.pusha();
	a.mov(eax, dword_ptr(edi, 0x5EC8));
	a.sub(eax, ptr_abs((DWORD)&m.modBaseAddr));
	a.mov(ebx, ptr_abs((DWORD)&mirror));
	a.add(ebx, eax);
	a.mov(dword_ptr(edi, 0x5EC8), ebx);
	a.popa();
	a.push(dword_ptr(edi, 0x5EC8));
	a.jmp((DWORD)GetModuleHandleA("protect") + 0xC6C723);

	DWORD protect = (DWORD)GetModuleHandleA("protect") + 0xC6C71D;
	patch.PlaceJit((LPVOID)protect, &a, 1);
}

void Modules::Protector::DisableCheckMemory()
{
	GetModuleInfo("AOgame", &m, true);

	mirror = malloc(m.modBaseSize);
	memcpy(mirror, m.modBaseAddr, m.modBaseSize);

	redirect();
}