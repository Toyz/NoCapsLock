#pragma once
#include <Windows.h>
#include <VersionHelpers.h>
#include <stdio.h>
#include <string.h>
#include "helpers.h"
#include "resource.h"
#include "defines.h"

class WindowHelpers
{
public:
	WindowHelpers();

	int CreateWndProc();
	void TaskbarNotify(HWND hWnd);
	HWND getHandler();

public:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
	static bool isNoCapsLockEnabled();
	static bool isWindowsKeyEnabled();
	static bool isMenuKeyEnabled();

private:
	static void ChangeTrayTitle(const char * title);
	HWND hwndWindow;

private:
	static NOTIFYICONDATA Tray;
	static bool ToolTipCreated;
	static bool isCapsLockingDisabled;
	static bool isWidowsKeyDisabled;
	static bool isMenuKeyDisabled;
};

