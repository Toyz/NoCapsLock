#include "helpers.h"

HWND helpers::GetConsoleWindow() {
	return FindWindow("ConsoleWindowClass", NULL);
}

HWND helpers::GetNoCapsLockWindow() {
	return FindWindow(NOCAPSCLASS, NULL);
}

bool helpers::CheckOneInstance() {
	HANDLE  m_hStartEvent = CreateEventW(NULL, FALSE, FALSE, L"Global\\CSAPP");

	if (m_hStartEvent == NULL)
	{
		CloseHandle(m_hStartEvent);
		return false;
	}


	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		CloseHandle(m_hStartEvent);
		m_hStartEvent = NULL;
		return false;
	}

	return true;
}

void helpers::DisableCapsLock() {
	keybd_event(VK_CAPITAL, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
	keybd_event(VK_CAPITAL, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
}