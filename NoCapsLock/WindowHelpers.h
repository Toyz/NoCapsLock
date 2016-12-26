#pragma once
#include <Windows.h>
#include <stdio.h>
#include "helpers.h"
#include "resource.h"
#include "defines.h"

class WindowHelpers
{
public:
	int CreateWndProc();
	void TaskbarNotify(HWND hWnd);
	HWND getHandler();
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

private:
	HWND hwndWindow;
};

