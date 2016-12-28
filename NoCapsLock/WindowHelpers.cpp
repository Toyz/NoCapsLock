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
	wc.hbrBackground = CreateSolidBrush(RGB(240, 240, 240));
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
		TEXT("Options - NoCapsLock"),
		WS_OVERLAPPED | WS_BORDER | WS_SYSMENU,
		520, 20, 550, 400,
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

void WindowHelpers::CreateUI(HWND hwnd) {
	std::map<DWORD_PTR, KeyObject::key_t> keys = KeyManager::GetKeyMap();
	std::map<DWORD_PTR, KeyObject::key_t>::iterator it;
	
	int startX = 2;
	for (it = keys.begin(); it != keys.end(); it++)
	{
		CreateWindow(TEXT("button"), TEXT(it->second.title.c_str()),
			WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
			2, startX, 400, 20,
			hwnd, (HMENU) it->first, NULL, NULL);

		CheckDlgButton(hwnd, it->first, it->second.enabled ? BST_CHECKED : BST_UNCHECKED);
		startX += 22;
	}

	HFONT defaultFont;
	defaultFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hwnd, WM_SETFONT, WPARAM(defaultFont), TRUE);
}

LRESULT CALLBACK WindowHelpers::WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message) {
	case WM_CREATE:
		CreateUI(hwnd);
		break;
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


#if _DEBUG
			InsertMenu(hMenu, 0, MFT_SEPARATOR, -1, "Show Options");
			InsertMenu(hMenu, 0, MFT_STRING, ID__SHOWOPTIONS, "Show Options");
#endif

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

		case ID__SHOWOPTIONS:
			ShowWindow(hwnd, 1);
			break;
		default:
			KeyObject::key_t KeyMeta = KeyManager::FindKey(wparam);

			if (KeyMeta.title != "") {
				KeyMeta.enabled = !KeyMeta.enabled;
				CheckDlgButton(hwnd, wparam, KeyMeta.enabled ? BST_CHECKED : BST_UNCHECKED);
				KeyManager::UpdateByKey(wparam, KeyMeta);
			}
		}

		break;
	case WM_CLOSE:
		//PostMessage(helpers::GetConsoleWindow(), WM_CLOSE, 0, 0);
		//PostQuitMessage(0);
		ShowWindow(hwnd, 0);
		return true;
	}

	return DefWindowProc(hwnd, message, wparam, lparam);
}

void WindowHelpers::ChangeTrayTitle(const char * title) {
	strcpy_s(Tray.szTip, title);
	Tray.uFlags = NIF_ICON | NIF_TIP | NIF_SHOWTIP | NIF_MESSAGE | NIF_GUID;
	Shell_NotifyIcon(NIM_MODIFY, &Tray);
}
