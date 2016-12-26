#define _WIN32_WINNT 0x0400
#pragma comment( lib, "user32.lib" )

#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <shellapi.h>
#include "defines.h"
#include "resource.h"
#include "helpers.h"
#include "WindowHelpers.h"

HHOOK hKeyboardHook;

__declspec(dllexport) LRESULT CALLBACK KeyboardEvent(int nCode, WPARAM wParam, LPARAM lParam)
{
	DWORD SHIFT_key = 0;
	DWORD CTRL_key = 0;
	DWORD ALT_key = 0;

	if ((nCode == HC_ACTION) && ((wParam == WM_SYSKEYDOWN) || (wParam == WM_KEYDOWN)))
	{
		KBDLLHOOKSTRUCT hooked_key = *((KBDLLHOOKSTRUCT*)lParam);
		DWORD dwMsg = 1;
		dwMsg += hooked_key.scanCode << 16;
		dwMsg += hooked_key.flags << 24;
		char lpszKeyName[1024] = { 0 };
	
		int i = GetKeyNameText(dwMsg, (lpszKeyName + 1), 0xFF) + 1;

		int key = hooked_key.vkCode;

		if (WindowHelpers::isNoCapsLockEnabled()) {
			if (hooked_key.vkCode == VK_CAPITAL) {
				if ((GetKeyState(VK_CAPITAL) & 0x0001) == 0) {
					helpers::DisableCapsLock();
				}
			}
		}

		SHIFT_key = GetAsyncKeyState(VK_SHIFT);
		CTRL_key = GetAsyncKeyState(VK_CONTROL);
		ALT_key = GetAsyncKeyState(VK_MENU);

		if (key >= 'A' && key <= 'Z')
		{

			if (GetAsyncKeyState(VK_SHIFT) >= 0) key += 32;

			// Lets you quit the program lmao
			if (CTRL_key != 0 && ALT_key != 0 && SHIFT_key != 0 && key == 'Q')
			{
				PostMessage(helpers::GetNoCapsLockWindow(), WM_CLOSE, 0, 0);
			}

			if (CTRL_key != 0 && ALT_key != 0 && SHIFT_key != 0 && key == 'D')
			{
				WindowHelpers::SetIsDisabled(false);
			}

			if (CTRL_key != 0 && ALT_key != 0 && SHIFT_key != 0 && key == 'E')
			{
				WindowHelpers::SetIsDisabled(true);
			}

			SHIFT_key = 0;
			CTRL_key = 0;
			ALT_key = 0;
		}

	}

	return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}

void MessageLoop()
{
	MSG message;
	while (GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

DWORD WINAPI CapsLockKillerHook(LPVOID lpParm)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	if (!hInstance) hInstance = LoadLibrary((LPCSTR)lpParm);
	if (!hInstance) return 1;

	hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardEvent, hInstance, NULL);
	MessageLoop();
	UnhookWindowsHookEx(hKeyboardHook);
	return 0;
}

void CreateWindowThread() {
	WindowHelpers winhelpers;
	winhelpers.CreateWndProc();
}

int main(int argc, char** argv)
{
	if (helpers::CheckOneInstance()) {
		HANDLE hThread;
		HANDLE wThread;
		DWORD dwThread;
		DWORD dwwThread;

		printf("Simple tool created by Toyz which allows you to kill capslock\n");
		printf("Source code at: https://github.com/Toyz/NoCapsLock\n");
		printf("LICENSED UNDER APACHE 2.0\n");

		hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CapsLockKillerHook, (LPVOID)argv[0], NULL, &dwThread);
		wThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CreateWindowThread, (LPVOID)argv[0], NULL, &dwwThread);

		ShowWindow(helpers::GetConsoleWindow(), SHOW_WINDOW);
		if (hThread) return WaitForSingleObject(hThread, INFINITE);
		if (wThread) return WaitForSingleObject(wThread, INFINITE);
		else return 1;
	}
	else {
		ShowWindow(helpers::GetConsoleWindow(), 0);
		MessageBox(HWND_DESKTOP, "Only one instance can be running at a given time!", "ERROR - NoCapsLock", MB_ICONERROR | MB_OK);
		return 0;
	}
}
