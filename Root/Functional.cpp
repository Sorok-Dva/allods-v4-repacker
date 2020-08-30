#include "Functional.h"
extern BP br;

//#pragma region XDB_HACK
//#include <io.h>
//bool FileExists(const char *fname)
//{
//	return access(fname, 0) != -1;
//}
//
//DWORD GetXdbAddr = 0x00461BF5;
//DWORD GetXdbOff = 0x00461BF9;
//
//void __declspec(naked) GetXdb(void) {
//	__asm {
//		mov[esi + eax * 4 + 0x0C], ebx
//		jmp[GetXdbOff]
//	}
//}
//
//void CheckXDBS(PCONTEXT db)
//{
//	DWORD struct_a = db->Esi + db->Eax * 4;
//	char * xdb = (char*)db->Ebx;
//	if (xdb == NULL) return;
//
//	string fname = "../Data/" + string(xdb);
//	if (FileExists(fname.c_str()))
//	{
//		*(DWORD*)(struct_a + 0x10) = 0x0;
//	}
//}
//#pragma endregion

//#pragma region REPLACE_XDB
//DWORD exitAddr = 0x0043AEE4;
//void __declspec(naked) ReplaceXdb()
//{
//	__asm
//	{
//		cmp eax, ebx
//		jmp[exitAddr]
//	}
//}
//
//DWORD pCont = 0x0;
//void CheckReplace(PCONTEXT db)
//{
//	if (db->Eax == NULL) return;
//
//	DWORD Status = *(DWORD*)(db->Eax + 0x10);
//	if (Status == 0x0)
//	{
//		pCont = db->Eax;
//		db->Eax = 0x0;
//	}
//}
//
//DWORD retAddr = 0x0043AF12;
//void __declspec(naked) Replace2()
//{
//	__asm {
//		mov edi, eax
//		jmp[retAddr]
//	}
//}
//
//void ReplaceWave2(PCONTEXT db)
//{
//	db->Eax = pCont;
//}
//
//void Wave2()
//{
//#if VERSION_ON
//	br.ClearBP(vAddr);
//#endif
//	br.ClearBP(GetXdbAddr);
//	br.ClearBP(0x0043AEE2);
//
//	br.AddHWBreak(0x0043AF10, &Replace2, ReplaceWave2);
//#if VERSION_ON
//	br.AddHWBreak(vAddr, &VersionHack, VersionSet);
//#endif
//	br.AddHWBreak(0x0043AEE2, &ReplaceXdb, CheckReplace);
//}
//#pragma endregion

void BreaksInit()
{
#if REPLACE_INIT
	br.AddHWBreak(GetXdbAddr, &GetXdb, CheckXDBS);
#endif
#if REPLACE_XDB
	br.AddHWBreak(0x0043AEE2, &ReplaceXdb, CheckReplace);
#endif
}