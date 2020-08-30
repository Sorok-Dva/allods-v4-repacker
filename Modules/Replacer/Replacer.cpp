#include "../Modules.h"

#define MOD    regex_constants::perl | boost::regex::no_mod_s | boost::regex::no_mod_m 
#define MODx   regex_constants::perl | boost::regex::no_mod_s | boost::regex::no_mod_m | regex_constants::mod_x
#define MODs   regex_constants::perl | boost::regex::no_mod_m | regex_constants::mod_s
#define MODxs  regex_constants::perl | boost::regex::no_mod_m | regex_constants::mod_s | regex_constants::mod_x

#define MODm   regex_constants::perl | boost::regex::no_mod_s  
#define MODxm  regex_constants::perl | boost::regex::no_mod_s| regex_constants::mod_x
#define MODsm  regex_constants::perl | regex_constants::mod_s
#define MODxsm regex_constants::perl | regex_constants::mod_s | regex_constants::mod_x

#define BOOST_REGEX_NON_RECURSIVE
#include "boost/regex.hpp"

string regexFile = "Replacer.txt";
string mainFile = "Replacer.txt";
map<DWORD, string> xdbs;
map<DWORD, bool> reps;
vector<string> listMask;

bool isRegex = false;

#include <io.h>
bool FileExists(const char *path)
{
	string fname = "../data/" + string(path);
	return access(fname.c_str(), 0) != -1;
}

bool regexCompare(string str, string regex)
{
	boost::regex reg(regex);
	return boost::regex_match(str, reg);
}

bool StrCmp(const char* str1, const char* str2)
{
	for (int i = 0; str1[i] && str2[i]; i++)
	{
		if (str1[i] == '*' || str2[i] == '*')
			return true;
		if (str1[i] != str2[i])
			return false;
	}
	return true;
}

void replace();
void msg(LPMSG msg)
{
	switch (msg->message)
	{
	case WM_HOTKEY:
		if (msg->wParam == 1)
			replace();
		break;
	default:
		break;
	}
}

void printHelp()
{
	printf("==========================================\n");
	printf("Replacer monitor started\n");
	printf("Use: Ctrl + Shift + R to replace resources\n");
	printf("MaskList of files: %s\n", mainFile.c_str());
	printf("==========================================\n");
}

void Modules::Replacer::StartMonitor()
{
	printHelp();
	RegisterHotKey(NULL, 1, MOD_NOREPEAT | MOD_SHIFT | MOD_CONTROL, 0x52);
}

void Modules::Replacer::HookMsgTranslator()
{
	ASM(a);
	a.call(ptr_abs(0x01314618));

	a.pusha();
	a.lea(eax, dword_ptr(ebp, -0x2C));
	a.push(eax);
	a.call((DWORD)&msg);
	a.add(esp, 0x4);
	a.popa();
	
	a.jmp(0x004A999E);
	patch.PlaceJit(0x004A9998, &a, 1);
}

void Modules::Replacer::SetFile(string fname)
{
	mainFile = fname;
}

#pragma region XDB_Getter
void addXdb(DWORD addr)
{
	char* xdb = (char*)*(DWORD*)(addr + 0x0C);
	xdbs.insert(pair<DWORD, string>(addr, string(xdb)));
}

void Modules::Replacer::EnableXDBGetter()
{
	ASM(a);
	a.mov(dword_ptr(esi, eax, 2, 0x0C), ebx);
	a.add(edi, 0x0C);

	a.pusha();
	a.mov(ecx, 4);
	a.mul(ecx);
	a.add(esi, eax);
	a.push(esi);
	a.call((DWORD)&addXdb);
	a.add(esp, 0x4);
	a.popa();

	a.jmp(0x00461BFC);
	patch.PlaceJit(0x00461BF5, &a, 2);
}
#pragma endregion

#pragma region XDB_Replacer
void readList()
{
	string s;
	ifstream file(isRegex ? regexFile : mainFile);
	if (!file.is_open())
		return;
	listMask.clear();
	while (getline(file, s))
	{
		if (s.size() == 0)
			continue;
		if (s.size() >= 2 && s[0] == '/' && s[1] == '/')
			continue;
		if (s[0] == ' ')
			continue;
		replace(s.begin(), s.end(), '\\', '/');
		listMask.push_back(s);
	}
	file.close();
}

#include <boost/algorithm/string.hpp>
bool getBool(string str)
{
	if (str.size() == 0)
		return false;
	if (str[0] == '1')
		return true;
	if (boost::iequals(str, "true"))
		return true;
	return false;
}

void readSettings()
{
	string s;
	ifstream file("Developer.txt");
	if (!file.is_open())
		return;
	while (getline(file, s))
	{
		if (s.find("Replacer=") != string::npos)
		{
			string rep = s.erase(0, s.rfind("=") + 1);
			Modules::Replacer::SetFile(rep);
		} 
		else if (s.find("Regex=") != string::npos)
		{
			string rep = s.erase(0, s.rfind("=") + 1);
			isRegex = getBool(rep);
		}
		else if (s.find("RegexFile=") != string::npos)
		{
			string reg = s.erase(0, s.rfind("=") + 1);
			regexFile = reg;
		}
	}
	file.close();
}

void compareRegexXDB()
{
	reps.clear();
	for(auto it = xdbs.begin(); it != xdbs.end(); ++it)
		for(int i = 0; i < listMask.size(); i++)
		{
			try
			{
				if (regexCompare(it->second, listMask[i]) && FileExists(it->second.c_str()))
				{
					*(DWORD*)(it->first + 0x10) = 0x0;
					reps.insert(pair<DWORD, bool>(it->first, true));
				}
			}
			catch (exception ex)
			{
				printf("Error: %s\n", ex.what());
				return;
			}
		}			
}

void compareXDB()
{
	reps.clear();
	for (auto it = xdbs.begin(); it != xdbs.end(); ++it)
		for (int i = 0; i < listMask.size(); i++)
		{
			try
			{
				if (StrCmp(it->second.c_str(), listMask[i].c_str()) && FileExists(it->second.c_str()))
				{
					*(DWORD*)(it->first + 0x10) = 0x0;
					reps.insert(pair<DWORD, bool>(it->first, true));
				}
			}
			catch (exception ex)
			{
				printf("Error: %s\n", ex.what());
				return;
			}
		}
}

void replace()
{
	system("cls");
	
	printHelp();
	printf("Starting File Replacer\n");
	readSettings();

	printf("Reading %s...\n", mainFile.c_str());
	readList();

	printf("Searching XDB for mask...\n");
	if (isRegex)
		compareRegexXDB();
	else
		compareXDB();

	printf("Replacer finished. Go to view resource.\n");
}

DWORD pOld = 0x0;

void ReplaceForStruct(PCONTEXT db)
{
	if (pOld == 0x0) return;

	DWORD check = *(DWORD*)(pOld + 0x10);
	if (check != 0)
	{
		return;
	}

	if (reps[pOld] && db->Eax != 0x0 && db->Eax != pOld)
	{
		DWORD ptr = *(DWORD*)pOld;
		static DWORD func = 0x0;
		func = *(DWORD*)(ptr/* + 0x14*/);
		static DWORD bak = 0x0;

		static DWORD func_clear = 0x0; 
		func_clear = *(DWORD*)(ptr + 0x14);
		DWORD(*getSize)() = reinterpret_cast<DWORD(*)(void)>(func_clear);
		memset((void*)(pOld + 0x10), 0x0, getSize() - 0x10);

		//static DWORD bak_bl = 0x0;
		//bak_bl = *(DWORD*)0x0159B5C8;
		__asm {
			pushad

			mov ecx, [pOld]
			mov ecx, [ecx + 0x0C]
			mov[bak], ecx

			mov ecx, [pOld]
			call func

			mov ecx, [pOld]
			lea ecx, [ecx + 0x0C]
			mov ebx, [bak]
			mov[ecx], ebx

			popad
		}
		*(DWORD*)0x0159B5C8 = 1;
		db->Eax = pOld;
		reps.erase(pOld);
	}
}

void AltPatch() // FORCE LOADING HREF'S FORCON
{
	X86Assembler a(&runtime);
	a.mov(ecx, ebx);
	a.jmp(0x00A0F10F);
	patch.PlaceJit(0x00A0F109, &a, 1);
}

void Patch2()
{
	X86Assembler a(&runtime);
	Label xdb = a.newLabel();

	a.mov(ecx, dword_ptr(esi, 0x28));
	a.mov(eax, dword_ptr(ecx));
	a.mov(edx, dword_ptr(eax, 0x8));
	a.push(edi);
	a.call(edx); // Search in pack.bin
	a.test(eax, eax);
	a.je(xdb);
	a.cmp(dword_ptr(eax, 0x10), 0x0);
	a.jne(xdb);
	a.mov(eax, 0x0);

	a.bind(xdb);
	a.jmp(0x004397DE);

	patch.PlaceJit(0x004397D3, &a);
}

void Replace()
{
	X86Assembler a(&runtime);

	Label Null = a.newLabel();
	Label Exit = a.newLabel();

	//AltPatch();
	a.mov(ptr_abs((DWORD)&pOld), eax);

	a.cmp(eax, 0x0);
	a.je(Null);

	a.cmp(dword_ptr(eax, 0x10), 0x0);
	a.jne(Exit);

	a.bind(Null);
	a.jmp(0x0043AEEA);

	a.bind(Exit);
	a.jmp(0x0043B0F4);

	patch.PlaceJit(0x0043AEE2, &a, 3);

	Patch2();
}

void Modules::Replacer::EnableReplacer()
{
	ASM(a);
	a.mov(edi, eax);
	a.jmp(0x0043AF12);
	br.AddINT3Break(0x0043AF10, patch.GetAsmJit(&a), ReplaceForStruct);
	Replace();
}
#pragma endregion