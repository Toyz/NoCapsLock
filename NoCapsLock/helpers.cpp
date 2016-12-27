#include "helpers.h"

HWND helpers::GetConsoleWindow() {
	return FindWindow("ConsoleWindowClass", NULL);
}

HWND helpers::GetNoCapsLockWindow() {
	return FindWindow(NOCAPSCLASS, NULL);
}

int helpers::getStringWidth(HWND win, const char *text, HFONT font) {
	HDC dc = GetDC(win);
	SelectObject(dc, font);

	RECT rect = { 0, 0, 0, 0 };
	DrawText(dc, text, strlen(text), &rect, DT_CALCRECT | DT_NOPREFIX | DT_SINGLELINE);
	int textWidth = abs(rect.right - rect.left);

	ReleaseDC(NULL, dc);

	printf("Font Width: %d\n", textWidth);
	return textWidth;
}

bool helpers::StringToBool(const std::string & s) {
	return s.at(0) == '1';
}

bool helpers::SaveResourceToFile(char *fn, int res) {
	HRSRC hrsrc = FindResourceEx(GetModuleHandle(NULL),
		TEXT("text"),
		MAKEINTRESOURCE(res),
		MAKELANGID(LANG_NEUTRAL,
			SUBLANG_NEUTRAL));

	if (hrsrc == NULL) return FALSE;
	DWORD size = SizeofResource(GetModuleHandle(NULL), hrsrc);
	HGLOBAL hglob = LoadResource(GetModuleHandle(NULL), hrsrc);
	LPVOID rdata = LockResource(hglob);
	HANDLE hFile =
		CreateFile(fn, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD writ; WriteFile(hFile, rdata, size, &writ, NULL);
	CloseHandle(hFile);
	return TRUE;

	return true;
}

unsigned int helpers::split(const std::string &txt, std::vector<std::string> &strs, char ch)
{
	int pos = txt.find(ch);
	int initialPos = 0;
	strs.clear();

	// Decompose statement
	while (pos != std::string::npos) {
		strs.push_back(txt.substr(initialPos, pos - initialPos + 1));
		initialPos = pos + 1;

		pos = txt.find(ch, initialPos);
	}

	// Add the last one
	strs.push_back(txt.substr(initialPos, MyMinTool(pos, static_cast<int>(txt.size())) - initialPos + 1));

	return strs.size();
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