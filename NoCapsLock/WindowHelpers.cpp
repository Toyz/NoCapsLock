#include "WindowHelpers.h"

HWND WindowHelpers::getHandler() {
	return hwndWindow;
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
		TEXT("the window"),
		WS_OVERLAPPED | WS_BORDER | WS_SYSMENU,
		520, 20, 300, 300,
		helpers::GetConsoleWindow(),
		NULL,

		hInstance, NULL);

	ShowWindow(hwndWindow, SW_SHOWNORMAL); 
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
			TrackPopupMenu((HMENU)GetSubMenu(LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU1)), 0), TPM_LEFTALIGN, cursor.x, cursor.y, 0, hwnd, NULL);
			break;
		}
		break;

	case WM_COMMAND:
		switch (wparam) {
		case ID__QUIT:
			PostMessage(helpers::GetConsoleWindow(), WM_CLOSE, 0, 0);
			PostQuitMessage(0);
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
