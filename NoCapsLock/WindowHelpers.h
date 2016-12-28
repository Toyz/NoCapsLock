#pragma once
#include <Windows.h>
#include <VersionHelpers.h>
#include <stdio.h>
#include <string.h>
#include <map>

#include "helpers.h"
#include "resource.h"
#include "defines.h"
#include "KeyManager.h"

class WindowHelpers
{
public:
	WindowHelpers();

	int CreateWndProc();
	void TaskbarNotify(HWND hWnd);
	HWND getHandler();

public:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

private:
	static HWND CreateAVerticalScrollBar(HWND hwndParent, int sbHeight);
	static void CreateUI(HWND hwnd);
	static void ChangeTrayTitle(const char * title);
	HWND hwndWindow;

private:
	static NOTIFYICONDATA Tray;
	static bool ToolTipCreated;
};

