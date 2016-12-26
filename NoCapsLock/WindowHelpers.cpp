#include "WindowHelpers.h"

bool WindowHelpers::isCapsLockingDisabled;

WindowHelpers::WindowHelpers() {
	isCapsLockingDisabled = true;
}

HWND WindowHelpers::getHandler() {
	return hwndWindow;
}

bool WindowHelpers::isNoCapsLockEnabled() {
	return isCapsLockingDisabled;
}

void WindowHelpers::SetIsDisabled(bool enabled) {
	isCapsLockingDisabled = enabled;
}

void WindowHelpers::TaskbarNotify(HWND hWnd) {
	NOTIFYICONDATA Tray;
	ShowWindow(hWnd, SHOW_WINDOW);
	Tray.cbSize = sizeof(Tray);
	Tray.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
	Tray.hWnd = hWnd;
	strcpy_s(Tray.szTip, "NoCapsLock - Running");
	Tray.uCallbackMessage = WM_MYMESSAGE;
	Tray.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	Tray.uID = 1;
	Shell_NotifyIcon(NIM_ADD, &Tray);
}

int WindowHelpers::CreateWndProc() {
	HINSTANCE hInstance = (HINSTANCE)GetWindowLong(helpers::GetConsoleWindow(), GWL_HINSTANCE);

	WNDCLASS wc = { 0 };
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = TEXT("NoCapsLockWindow");
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClass(&wc))
	{
		printf("Problem with WNDClass\n");
		return 1;
	}

	hwndWindow = CreateWindow(TEXT("NoCapsLockWindow"),
		TEXT("NoCapsLockWindow - Window"),
		WS_OVERLAPPED | WS_BORDER | WS_SYSMENU,
		520, 20, 300, 300,
		helpers::GetConsoleWindow(),
		NULL,

		hInstance, NULL);

	UpdateWindow(hwndWindow);
	TaskbarNotify(hwndWindow);

	MSG msg;
	while (GetMessage(&msg, hwndWindow, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

LRESULT CALLBACK WindowHelpers::WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message) {
	case WM_MYMESSAGE:
		switch (lparam) {
		case WM_RBUTTONUP:
			POINT cursor;
			GetCursorPos(&cursor);
			HMENU hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU1));

			SetForegroundWindow(hwnd);
			if (isCapsLockingDisabled) {
				CheckMenuItem(hMenu, ID__BLOCKCAPSLOCK, MF_CHECKED);
			}
			else {
				CheckMenuItem(hMenu, ID__BLOCKCAPSLOCK, MF_UNCHECKED);
			}

			TrackPopupMenu((HMENU)GetSubMenu(hMenu, 0), TPM_LEFTALIGN, cursor.x, cursor.y, 0, hwnd, NULL);
			break;
		}
		break;

	case WM_COMMAND:
		switch (wparam) {
		case ID__QUIT:
			PostMessage(helpers::GetConsoleWindow(), WM_CLOSE, 0, 0);
			PostQuitMessage(0);
			break;
		case ID__BLOCKCAPSLOCK:
			isCapsLockingDisabled = !isCapsLockingDisabled;

			if (isCapsLockingDisabled) {
				helpers::DisableCapsLock();
			}
		case ID__SOURCECODE:
			ShellExecute(0, 0, "https://github.com/Toyz/NoCapsLock", 0, 0, SW_SHOW);
			break;
		}

		break;
	case WM_DESTROY:
		PostMessage(helpers::GetConsoleWindow(), WM_CLOSE, 0, 0);
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, message, wparam, lparam);
}
