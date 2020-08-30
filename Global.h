#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <TlHelp32.h>

using namespace std;

#include "Patcher.h"
#include "BreakPoints/BP.h"

#include "Root\Exceptions.h"
#include "Root\Functional.h"

#define StartRoutine(Function) CreateThread(0,0,(LPTHREAD_START_ROUTINE)Function,0,0,0);