#define _WIN32_WINNT 0x0400
#pragma comment( lib, "user32.lib" )

#include <iostream>
#include <windows.h>
#include <stdio.h>

HHOOK hKeyboardHook;

__declspec(dllexport) LRESULT CALLBACK KeyboardEvent(int nCode, WPARAM wParam, LPARAM lParam)
{
	if ((nCode == HC_ACTION) && ((wParam == WM_SYSKEYDOWN) || (wParam == WM_KEYDOWN)))
	{
		KBDLLHOOKSTRUCT hooked_key = *((KBDLLHOOKSTRUCT*)lParam);
		DWORD dwMsg = 1;
		dwMsg += hooked_key.scanCode << 16;
		dwMsg += hooked_key.flags << 24;
		char lpszKeyName[1024] = { 0 };
	
		int i = GetKeyNameText(dwMsg, (lpszKeyName + 1), 0xFF) + 1;

		int key = hooked_key.vkCode;

		if (hooked_key.vkCode == VK_CAPITAL) {
			if ((GetKeyState(VK_CAPITAL) & 0x0001) == 0) {
				keybd_event(VK_CAPITAL, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
				keybd_event(VK_CAPITAL, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
			}
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

int main(int argc, char** argv)
{
	HANDLE hThread;
	DWORD dwThread;
	printf("Simple tool created by Toyz which allows you to kill capslock\n");
	printf("Source code at: https://github.com/Toyz/NoCapsLock\n");
	printf("LICENSED UNDER APACHE 2.0\n");

	hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CapsLockKillerHook, (LPVOID)argv[0], NULL, &dwThread);

#ifndef _DEBUG
	ShowWindow(FindWindowA("ConsoleWindowClass", NULL), false);
#endif // !DEBUG



	if (hThread) return WaitForSingleObject(hThread, INFINITE);
	else return 1;

}
