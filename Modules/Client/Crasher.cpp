#include "../Modules.h"

void PatchTimeOut()
{
	X86Assembler a(&runtime);
	a.jmp(0x00E9A925);
	patch.PlaceJit(0x00E9A91F, &a, 1);

	printf("TimeOut Disabled\n");
}

void PatchLags()
{
	X86Assembler a(&runtime);
	a.jmp(0x00F27366);
	patch.PlaceJit(0x00F2735A, &a, 1);

	printf("Talent Lags Disabled\n");
}

//============================
DWORD returnA = 0x00E9ADBC;
DWORD callA = 0x00F57CE0;
DWORD dump = 0x0;

void __declspec(naked) PacketWhile()
{
	__asm
	{
		mov eax, [dump]
		push eax
		lea ecx, [esi + 0x0C]
		call[callA]
		mov eax, 0x0
		jmp[returnA]
	}
}

DWORD normalA = 0x00E9ADC5;
void __declspec(naked) NormalPacket()
{
	__asm
	{
		push eax
		lea ecx, [esi + 0x0C]
		call[callA]
		jmp[normalA]
	}
}

int counter = 0;
int max_counter = 20;

void Packets(PCONTEXT db)
{
	if (*(DWORD*)(db->Ebp + 0x4) != 0x00502163)
	{
		db->Eip = (DWORD)&NormalPacket;
		return;
	}

	if (db->Eax != 0x0)
		dump = db->Eax;
	if(counter < max_counter)
	{
		counter++;
		db->Eip = (DWORD)&PacketWhile;
		return;
	}
	else
	{
		counter = 0;
		db->Eip = 0x00E9ADC5;
		return;
	}
}

void ReadCounter(string fname = "counter.txt")
{
	string s;
	ifstream file(fname);
	getline(file, s);
	max_counter = stoi(s);
	file.close();
}

void Redeemer()
{
	br.AddEipINT3Break(0x00E9ADBC, Packets);
	ReadCounter();

	int mb = max_counter * 14 / 1024 / 1024;

	int kb = max_counter * 14 / 1024 % 1024;

	int b = max_counter * 14 % 1024;

	cout << "========COUNTER========" << endl;
	cout << "Number of kickbacks: " << max_counter << endl;
	printf("Size: %d MB, %d KB, %d B\n", mb, kb, b);
	//cout << "MB: " << mb << endl;
	//cout << "KB: " << kb << endl;
	//cout << "B: " << b << endl;
	printf("Will be sent\n");
}

void CrasherEnable()
{
#if CRASHER_VERSION
	PatchTimeOut();
	PatchLags();
	Redeemer();
#endif
}