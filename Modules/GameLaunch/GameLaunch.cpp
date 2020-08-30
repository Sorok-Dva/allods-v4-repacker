#include "../Modules.h"

void Modules::GameLaunch::HackHash()
{
	X86Assembler a(&runtime);
	a.mov(al, 1);
	a.jmp(0x004030C5);
	br.AddHWBreak(0x004030BB, patch.GetAsmJit(&a));
}

void Modules::GameLaunch::HackName()
{
	X86Assembler a(&runtime);
	a.mov(byte_ptr(ebp, -0x1D), 1);
	a.jmp(0x00403046);
	br.AddHWBreak(0x00403042, patch.GetAsmJit(&a));
}

void Modules::GameLaunch::HackFiles()
{
	X86Assembler a(&runtime);
	a.jmp(0x00F69DA5);
	br.AddHWBreak(0x00F69D64, patch.GetAsmJit(&a));
}

void Modules::GameLaunch::ClearHacks()
{
	br.ClearBP(0x004030BB);
	br.ClearBP(0x00403042);
}

void Modules::GameLaunch::ClearFiles()
{
	br.ClearBP(0x00F69D64);
}
