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