#pragma once
#include <Windows.h>
#include <VersionHelpers.h>
#include <stdio.h>
#include <string.h>
#include <map>

#include "helpers.h"
#include "TaskbarNotifierHandler.h"
#include "ContextMenuHandler.h"
#include "resource.h"
#include "defines.h"
#include "KeyManager.h"

class WindowsHandler
{
public:
	WindowsHandler();

	int CreateWndProc();
	HWND getHandler();

public:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

private:
	HWND hwndWindow;

private:
	static void CreateCustomMenuOptions(HWND hwnd);
};

