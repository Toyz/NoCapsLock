#include "WindowsHandler.h"

TaskbarNotifierHandler notifier;
ContextMenuHandler _ctxHandler;
RECT winSize{ 0, 310, 0, 400 };

WindowsHandler::WindowsHandler() {
}

HWND WindowsHandler::getHandler() {
	return hwndWindow;
}

int WindowsHandler::CreateWndProc() {
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

	std::string title = helpers::GetString(IDS_TITLE);
	title += " - Options";

	int xPos = (GetSystemMetrics(SM_CXSCREEN) - winSize.bottom) / 2;
	int yPos = (GetSystemMetrics(SM_CYSCREEN) - winSize.top) / 2;

	hwndWindow = CreateWindow(TEXT(NOCAPSCLASS),
		TEXT(title.c_str()),
		WS_OVERLAPPED | WS_BORDER | WS_SYSMENU | WS_MINIMIZEBOX,
		xPos , yPos, winSize.top, winSize.bottom,
		NULL,
		LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU2)),
		hInstance, NULL);

	UpdateWindow(hwndWindow);
	TaskbarNotifierHandler nf(hwndWindow);
	notifier = nf;

	ContextMenuHandler ctx(hwndWindow);
	_ctxHandler = ctx;
	_ctxHandler.CreateMenu();

	notifier.Create();

	MSG msg;
	while (GetMessage(&msg, hwndWindow, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

void WindowsHandler::CreateCustomMenuOptions(HWND hwnd) {
	int startX = 3;
	for (std::pair<DWORD_PTR, KeyObject::key_t> it : KeyManager::AllKeys())
	{
		CreateWindow(TEXT("button"), TEXT(it.second.title.c_str()),
			WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
			3, startX, winSize.top - 6, 20,
			hwnd, (HMENU) it.first, NULL, NULL);

		CheckDlgButton(hwnd, static_cast<int>(it.first), it.second.enabled ? BST_CHECKED : BST_UNCHECKED);
		startX += 23;
	}

	HFONT defaultFont;
	defaultFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hwnd, WM_SETFONT, WPARAM(defaultFont), TRUE);
}

LRESULT CALLBACK WindowsHandler::WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message) {
	case WM_CREATE:
		CreateCustomMenuOptions(hwnd);
		break;

	case WM_CONTEXTMSGEVENT:
		switch (lparam) {
		case WM_RBUTTONUP:
			POINT cursor;
			GetCursorPos(&cursor);
			_ctxHandler.Show(cursor);
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
			KeyObject::key_t KeyMeta = KeyManager::Find(wparam);

			if (KeyMeta.title != "") {
				KeyMeta.enabled = !KeyMeta.enabled;
				CheckDlgButton(hwnd, static_cast<int>(wparam), KeyMeta.enabled ? BST_CHECKED : BST_UNCHECKED);
				_ctxHandler.CheckItem(static_cast<int>(wparam), KeyMeta.enabled);
				KeyManager::Update(wparam, KeyMeta);
			}
			break;
		}

		break;

	case WM_CLOSE:
		ShowWindow(hwnd, 0);
		return 1;
	}

	return DefWindowProc(hwnd, message, wparam, lparam);
}

