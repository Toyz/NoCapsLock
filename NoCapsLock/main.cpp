#define _WIN32_WINNT 0x0601
#pragma comment( lib, "user32.lib" )

#include <windows.h>
#include <stdio.h>
#include <shellapi.h>

#include "ConfigHandler.h"
#include "defines.h"
#include "resource.h"
#include "helpers.h"
#include "WindowsHandler.h"
#include "KeyManager.h"
#include "KeyObject.h"

HHOOK hKeyboardHook;
ConfigHandler _cfgHandler;
HANDLE hThread;
HANDLE wThread;
DWORD dwThread;
DWORD dwwThread;


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

#if _DEBUG
		printf("Key = %d\n", key);
#endif

		KeyObject::key_t key_st = KeyManager::FindKey(hooked_key.vkCode);

		if (key_st.title != "") {
			if (key_st.enabled) {
				return 1;
			}
		}

		SHIFT_key = GetAsyncKeyState(VK_SHIFT);
		CTRL_key = GetAsyncKeyState(VK_CONTROL);
		ALT_key = GetAsyncKeyState(VK_MENU);

		if (key >= 'A' && key <= 'Z')
		{
			if (GetAsyncKeyState(VK_SHIFT) >= 0) key += 32;


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
	WindowsHandler winhelpers;
	winhelpers.CreateWndProc();
}

BOOL WINAPI ConsoleHandlerRoutine(DWORD dwCtrlType) {
	switch (dwCtrlType) {
	case CTRL_CLOSE_EVENT:
		_cfgHandler.Save();
		break;
	}

	return true;
}

int main(int argc, char** argv)
{
	if (helpers::CheckOneInstance()) {
		if (!helpers::FileExist(CONFIGFILE)) {
			helpers::SaveResourceToFile(CONFIGFILE, IDR_TEXT1);
		}

		ConfigHandler _cfg(CONFIGFILE);
		_cfgHandler = _cfg;
		_cfgHandler.Load();

		BOOL ret = SetConsoleCtrlHandler(ConsoleHandlerRoutine, TRUE);
	
		printf("Debug Output\n"); 
		printf("------------\n");

		hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CapsLockKillerHook, (LPVOID)argv[0], NULL, &dwThread);
		wThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CreateWindowThread, (LPVOID)argv[0], NULL, &dwwThread);

		ShowWindow(helpers::GetConsoleWindow(), 0);
		if (hThread) return WaitForSingleObject(hThread, INFINITE);
		if (wThread) return WaitForSingleObject(wThread, INFINITE);
		else return 1;
	} else {
		ShowWindow(helpers::GetConsoleWindow(), 0);
		MessageBox(HWND_DESKTOP, "Only one instance can be running at a given time!", "ERROR - NoCapsLock", MB_ICONERROR | MB_OK);
		return 0;
	}
}
