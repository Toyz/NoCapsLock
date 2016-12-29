#include "TaskbarNotifier.h"

TaskbarNotifier::TaskbarNotifier(HWND parent)
{
	_parent = parent;
}

TaskbarNotifier::~TaskbarNotifier()
{
	// todo
}

void TaskbarNotifier::Create() {
	if (_isCreated) return;

	ShowWindow(_parent, SHOW_WINDOW);

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
	Tray.hWnd = _parent;
	strcpy_s(Tray.szTip, helpers::GetString(IDS_TITLE));
	Tray.uCallbackMessage = WM_CONTEXTMSGEVENT;
	Tray.uFlags = NIF_ICON | NIF_TIP | NIF_SHOWTIP | NIF_MESSAGE | NIF_GUID;
	Tray.uID = 01;
	Shell_NotifyIcon(NIM_ADD, &Tray);
	_isCreated = true;
}

void TaskbarNotifier::ChangeTitle(const char * title) {
	if (!_isCreated) return;

	strcpy_s(Tray.szTip, title);
	Tray.uFlags = NIF_ICON | NIF_TIP | NIF_SHOWTIP | NIF_MESSAGE | NIF_GUID;
	Shell_NotifyIcon(NIM_MODIFY, &Tray);
}
