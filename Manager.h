#pragma once
#include "Global.h"
#include "Modules/Modules.h"

#define SHOW_CONSOLE 1
#define PRINT_FILES 0

#define PAUSE 0

#define REPLACE_INIT 0
#define REPLACE_XDB 0

#define VERSION_ON 0

#define CRASHER_VERSION 1

void Manager();
void ReadSettings(string fname);

extern int ClientVersion;