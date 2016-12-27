#pragma once
#define NOMINMAX
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

	template<class T>
	static const T& MyMinTool(const T& a, const T& b)
	{
		return (b < a) ? b : a;
	}
};

