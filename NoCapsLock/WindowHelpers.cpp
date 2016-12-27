#include "WindowHelpers.h"

UINT32 WindowHelpers::id;
bool WindowHelpers::isCapsLockingDisabled;
bool WindowHelpers::ToolTipCreated = false;
NOTIFYICONDATA WindowHelpers::Tray;

WindowHelpers::WindowHelpers() {
	isCapsLockingDisabled = false;
	id = 0;
}

HWND WindowHelpers::getHandler() {
	return hwndWindow;
}

bool WindowHelpers::isNoCapsLockEnabled() {
	return isCapsLockingDisabled;
}

void WindowHelpers::SetIsDisabled(bool enabled) {
	isCapsLockingDisabled = enabled;

	if (!isCapsLockingDisabled) {
		ChangeTrayTitle("NoCapsLock - Running");
		TooltipBallon("Caps lock key is now disabled", "NoCapsLock - Stopped");
	} else {
		ChangeTrayTitle("NoCapsLock - Stopped");
		TooltipBallon("Caps lock key is now enabled", "NoCapsLock - Running");
	}

}

void WindowHelpers::TooltipBallon(const char * text, const char * title) {
	Tray.dwInfoFlags = NIIF_USER | NIIF_INFO;
	Tray.uTimeout = 3500;

	strcpy_s(Tray.szInfo, sizeof(Tray.szInfo), text);
	strcpy_s(Tray.szInfoTitle, sizeof(Tray.szInfoTitle), title);
	/*if (IsWindowsVistaOrGreater())
		Tray.hBalloonIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));*/
	Tray.uFlags |= NIF_REALTIME;

	Shell_NotifyIcon(NIM_MODIFY | NIM_SETFOCUS, &Tray);
}

void WindowHelpers::TaskbarNotify(HWND hWnd) {
	if (!ToolTipCreated) {
		ShowWindow(hWnd, SHOW_WINDOW);
		//Tray.cbSize = NOTIFYICONDATA_V2_SIZE;

		if (IsWindowsVistaOrGreater()) {
			Tray.cbSize = sizeof(Tray);
			Tray.uVersion = NOTIFYICON_VERSION_4;
		}

		else if (IsWindowsXPOrGreater()) {
			Tray.cbSize = NOTIFYICONDATA_V3_SIZE;
			Tray.uVersion = NOTIFYICON_VERSION;
		}

		Shell_NotifyIcon(NIM_SETVERSION, &Tray);

		Tray.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		Tray.hWnd = hWnd;
		strcpy_s(Tray.szTip, "NoCapsLock - Running");
		Tray.uCallbackMessage = WM_CONTEXTMSGEVENT;
		Tray.uFlags = NIF_ICON | NIF_INFO | NIF_TIP | NIF_SHOWTIP | NIF_MESSAGE | NIF_GUID;
		Tray.uID = id;
		Shell_NotifyIcon(NIM_ADD, &Tray);
		ToolTipCreated = true;
	}
}

int WindowHelpers::CreateWndProc() {
	HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(helpers::GetConsoleWindow(), GWLP_HINSTANCE);

	WNDCLASS wc = { 0 };
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = TEXT(NOCAPSCLASS);
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClass(&wc))
	{
		printf("Problem with WNDClass\n");
		return 1;
	}

	hwndWindow = CreateWindow(TEXT(NOCAPSCLASS),
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
	case WM_CONTEXTMSGEVENT:
		switch (lparam) {
		case WM_RBUTTONUP:
			POINT cursor;
			GetCursorPos(&cursor);
			HMENU hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU1));

			SetForegroundWindow(hwnd);
			if (isCapsLockingDisabled) {
				CheckMenuItem(hMenu, ID__BLOCKCAPSLOCK, MF_UNCHECKED);
			}
			else {
				CheckMenuItem(hMenu, ID__BLOCKCAPSLOCK, MF_CHECKED);
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

			if (!isCapsLockingDisabled) {
				ChangeTrayTitle("NoCapsLock - Running");
				helpers::DisableCapsLock();
			} else {
				ChangeTrayTitle("NoCapsLock - Stopped");
			}

			break;

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

void WindowHelpers::ChangeTrayTitle(const char * title) {
	strcpy_s(Tray.szTip, title);
	Shell_NotifyIcon(NIM_MODIFY, &Tray);
}
