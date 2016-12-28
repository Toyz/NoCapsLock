#pragma once
#define NOMINMAX
#include <windows.h>
#include <stdio.h>
#include <vector>
#include <fstream>


#include "defines.h"

class helpers
{
private:
	static TCHAR szBuffer[256];
public:
	static char * GetString(int id);

	static HWND GetConsoleWindow();
	static HWND GetNoCapsLockWindow();
	static bool CheckOneInstance();
	static bool StringToBool(const std::string & s);
	static unsigned int split(const std::string &txt, std::vector<std::string> &strs, char ch);
	static bool SaveResourceToFile(char *fn, int res);
	static bool FileExist(const char *fileName)
	{
		std::ifstream infile(fileName);
		return infile.good();
	}

	template<class T>
	static const T& MyMinTool(const T& a, const T& b)
	{
		return (b < a) ? b : a;
	}
};

