#pragma once
#include <algorithm>
#include <windows.h>
#include <stdio.h>
#include <vector>


#include "defines.h"

class helpers
{
public:
	static HWND GetConsoleWindow();
	static HWND GetNoCapsLockWindow();
	static bool CheckOneInstance();
	static int getStringWidth(HWND win, const char *text, HFONT font);
	static bool StringToBool(const std::string & s);
	static unsigned int split(const std::string &txt, std::vector<std::string> &strs, char ch);
};

