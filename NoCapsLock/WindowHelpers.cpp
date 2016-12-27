#include "WindowHelpers.h"

bool WindowHelpers::ToolTipCreated = false;
NOTIFYICONDATA WindowHelpers::Tray;

WindowHelpers::WindowHelpers() {
}

HWND WindowHelpers::getHandler() {
	return hwndWindow;
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
		strcpy_s(Tray.szTip, "NoCapsLock");
		Tray.uCallbackMessage = WM_CONTEXTMSGEVENT;
		Tray.uFlags = NIF_ICON | NIF_TIP | NIF_SHOWTIP | NIF_MESSAGE | NIF_GUID;
		Tray.uID = 01;
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

			std::map<DWORD_PTR, KeyObject::key_t> keys = KeyManager::GetKeyMap();
			std::map<DWORD_PTR, KeyObject::key_t>::iterator it;
			for (it = keys.begin(); it != keys.end(); it++)
			{
				InsertMenu(hMenu, 0, MFT_STRING, it->first, it->second.title.c_str());
				CheckMenuItem(hMenu, it->first, it->second.enabled ? MF_CHECKED : MF_UNCHECKED);
			}

			SetForegroundWindow(hwnd);
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
		case ID__SOURCECODE:
			ShellExecute(0, 0, "https://github.com/Toyz/NoCapsLock", 0, 0, SW_SHOW);
			break;

		default:
			KeyObject::key_t KeyMeta = KeyManager::FindKey(wparam);

			if (KeyMeta.title != "") {
				KeyMeta.enabled = !KeyMeta.enabled;
				KeyManager::UpdateByKey(wparam, KeyMeta);
			}
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
	Tray.uFlags = NIF_ICON | NIF_TIP | NIF_SHOWTIP | NIF_MESSAGE | NIF_GUID;
	Shell_NotifyIcon(NIM_MODIFY, &Tray);
}
