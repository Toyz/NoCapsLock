#pragma once
#include <Windows.h>
#include <stdio.h>
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
	static void SetIsDisabled(bool enabled);

private:
	HWND hwndWindow;

private:
	static bool isCapsLockingDisabled;
};

