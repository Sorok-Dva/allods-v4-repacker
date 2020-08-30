#include "../Modules.h"

void clearBps()
{
	Modules::GameLaunch::ClearHacks();
}

void Modules::Stages::CrFileA()
{
	StartRoutine(clearBps);
	Protector::DisableCheckMemory();
	ReadSettings("Developer.txt");

	Client::DisableCheckFiles();
	Client::EnableProtectedLua();
	Client::DisableAbort();
	Replacer::EnableXDBGetter();
}

extern void CrasherEnable();
void Modules::Stages::CrWindow()
{
	CrasherEnable();
	Replacer::HookMsgTranslator();
	Replacer::StartMonitor();
	Replacer::EnableReplacer();
}
