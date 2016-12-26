#pragma once
#include <windows.h>
#include "defines.h"

class helpers
{
public:
	static HWND GetConsoleWindow();
	static HWND GetNoCapsLockWindow();
	static bool CheckOneInstance();
};

