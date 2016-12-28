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
		strcpy_s(Tray.szTip, helpers::GetString(IDS_TITLE));
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
	wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
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
		TEXT("Options"),
		WS_OVERLAPPED | WS_BORDER | WS_SYSMENU | WS_MINIMIZEBOX,
		520, 20, 300, 400,
		NULL,
		LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU2)),
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

void WindowHelpers::CreateCustomMenuOptions(HWND hwnd) {
	std::map<DWORD_PTR, KeyObject::key_t> keys = KeyManager::GetKeyMap();
	std::map<DWORD_PTR, KeyObject::key_t>::iterator it;
	
	int startX = 3;
	for (it = keys.begin(); it != keys.end(); it++)
	{
		CreateWindow(TEXT("button"), TEXT(it->second.title.c_str()),
			WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
			3, startX, 300 - 6, 20,
			hwnd, (HMENU) it->first, NULL, NULL);

		CheckDlgButton(hwnd, static_cast<int>(it->first), it->second.enabled ? BST_CHECKED : BST_UNCHECKED);
		startX += 23;
	}

	HFONT defaultFont;
	defaultFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hwnd, WM_SETFONT, WPARAM(defaultFont), TRUE);
}

void WindowHelpers::CreateMenuNotifyContext(HWND hwnd) {
	POINT cursor;
	GetCursorPos(&cursor);
	HMENU hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU1));

	std::map<DWORD_PTR, KeyObject::key_t> keys = KeyManager::GetKeyMap();
	std::map<DWORD_PTR, KeyObject::key_t>::iterator it;
	for (it = keys.begin(); it != keys.end(); it++)
	{
		InsertMenu(hMenu, 0, MFT_STRING, it->first, it->second.title.c_str());
		CheckMenuItem(hMenu, static_cast<int>(it->first), it->second.enabled ? MF_CHECKED : MF_UNCHECKED);
	}

	InsertMenu(hMenu, 0, MFT_SEPARATOR, -1, "-");
	InsertMenu(hMenu, 0, MFT_STRING, ID__SHOWOPTIONS, helpers::GetString(IDS_SHOWOPTIONS));

#if _DEBUG
	InsertMenu(hMenu, 0, MFT_SEPARATOR, -1, "-");
	InsertMenu(hMenu, 0, MFT_STRING, 0x9293, helpers::GetString(IDS_SHOWCONSOLE));
#endif

	SetForegroundWindow(hwnd);
	TrackPopupMenu((HMENU)GetSubMenu(hMenu, 0), TPM_LEFTALIGN, cursor.x, cursor.y, 0, hwnd, NULL);
}

LRESULT CALLBACK WindowHelpers::WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message) {
	case WM_CREATE:
		CreateCustomMenuOptions(hwnd);
		break;

	case WM_CONTEXTMSGEVENT:
		switch (lparam) {
		case WM_RBUTTONUP:
			CreateMenuNotifyContext(hwnd);
			break;
		}
		break;

	case WM_COMMAND:
		switch (wparam) {
		case ID_FILE_QUT:
		case ID__QUIT: {
			int msgboxID = MessageBox(
				NULL,
				"Are you sure you wish to quit the progam?",
				"Are you sure?",
				MB_ICONEXCLAMATION | MB_YESNO
			);

			if (msgboxID == IDYES) {
				PostMessage(helpers::GetConsoleWindow(), WM_CLOSE, 0, 0);
				PostQuitMessage(0);
			}
			break;
		}

		case 0x9293:
			ShowWindow(helpers::GetConsoleWindow(), 1);
			break;

		case ID__SOURCECODE:
			ShellExecute(0, 0, "https://github.com/Toyz/NoCapsLock", 0, 0, SW_SHOW);
			break;

		case ID__SHOWOPTIONS:
			ShowWindow(hwnd, 1);
			break;

		case ID_HELP_ABOUT:
			MessageBox(
				NULL,
				"NoCapsLock is created by\n\nToyz\n\nSourse code is located on github",
				"About",
				MB_OK
			);

			break;

		default:
			KeyObject::key_t KeyMeta = KeyManager::FindKey(wparam);

			if (KeyMeta.title != "") {
				KeyMeta.enabled = !KeyMeta.enabled;
				CheckDlgButton(hwnd, static_cast<int>(wparam), KeyMeta.enabled ? BST_CHECKED : BST_UNCHECKED);
				KeyManager::UpdateByKey(wparam, KeyMeta);
			}
		}

		break;

	case WM_CLOSE:
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
