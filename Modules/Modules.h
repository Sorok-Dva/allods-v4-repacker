#pragma once
#include "../Manager.h"

extern BP br;
extern Patcher patch;

namespace Modules
{
	namespace GameLaunch
	{
		void HackHash();
		void HackName();
		void HackFiles();
		void ClearHacks();
		void ClearFiles();
	}
	namespace Protector
	{
		void DisableCheckMemory();
	}
	namespace Client
	{
		void SetMainWindow(HWND hwnd);
		HWND GetMainWindow();
		void SetVersion(int version);
		void DisableCheckFiles();
		void EnableProtectedLua();
		void DisableAbort();
	}
	namespace Stages
	{
		void CrFileA();
		void CrWindow();
	}
	namespace Replacer
	{
		void StartMonitor();
		void HookMsgTranslator();
		void SetFile(string name);

		void EnableXDBGetter();
		void EnableReplacer();
	}
}