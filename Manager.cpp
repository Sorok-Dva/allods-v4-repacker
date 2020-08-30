#include "Manager.h"
#include "Root\API.h"

BP br;
Patcher patch;

void Manager()
{
	Mhook_SetHook(&(PVOID&)CrFileA, NewCrFileA);
	Mhook_SetHook(&(PVOID&)OriSysTime, SysTime);
	Mhook_SetHook(&(PVOID&)Window, NewWindow);
	Modules::GameLaunch::HackHash();
	Modules::GameLaunch::HackName();
}

void ReadSettings(string fname)
{
	string s;
	ifstream file(fname);
	if (!file.is_open())
		return;
	while (getline(file, s))
	{
#if VERSION_ON
		if (s.find("ClientVersion=") != string::npos)
		{
			string ver = s.erase(0, s.rfind("=") + 1);
			Modules::Client::SetVersion(stoi(ver));
		}
#endif	
		if (s.find("Replacer=") != string::npos)
		{
			string rep = s.erase(0, s.rfind("=") + 1);
			Modules::Replacer::SetFile(rep);
		}
	}
	file.close();
}