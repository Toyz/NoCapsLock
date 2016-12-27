#include "SystemTrayIcon.h"

bool SystemTrayIcon::Create(const std::wstring &    Tip,
	HWND                    hWndParent,
	const GUID &            Guid,
	UINT                    IdCallback,
	HICON                   hIcon,
	bool                    bSharedIcon,
	bool                    bHidden)
{
	if (bInitialized) return false;

	NotifyIconData.cbSize = sizeof(NotifyIconData);
	NotifyIconData.hWnd = hWndParent;
	NotifyIconData.uID = 0;    // Use GUID instead. (NIF_GUID)
	NotifyIconData.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP | NIF_STATE | NIF_GUID | NIF_SHOWTIP;
	NotifyIconData.uCallbackMessage = IdCallback;
	NotifyIconData.hIcon = hIcon;

	if (Tip.length() > MAX_TIP)
	{
		std::wcsncpy(NotifyIconData.szTip, Tip.c_str(), MAX_TIP);
		NotifyIconData.szTip[MAX_TIP] = wchar_t(0);
	}
	else
	{
		std::wcsncpy(NotifyIconData.szTip, Tip.c_str(), Tip.length() + 1);
	}

	NotifyIconData.dwState = (bSharedIcon ? NIS_SHAREDICON : 0)
		| (bHidden ? NIS_HIDDEN : 0);
	NotifyIconData.dwStateMask = NIS_SHAREDICON | NIS_HIDDEN;

	NotifyIconData.guidItem = Guid;

	bInitialized = Shell_NotifyIconW(NIM_ADD, &NotifyIconData);

	NotifyIconData.uVersion = NOTIFYICON_VERSION_4;
	bLastResult = Shell_NotifyIconW(NIM_SETVERSION, &NotifyIconData);

	return bInitialized;
}

void SystemTrayIcon::Balloon(const std::wstring &    Title,
	const std::wstring &    Message,
	HICON                   hBalloonIcon,
	BALLOON_ICON_TYPE       IconType,
	UINT                    Timeout,    // In milliseconds.
	bool                    bSound,
	bool                    bLargeIcon,
	bool                    bRespectQuiteTime)
{
	if (!bInitialized) return;

	if (Title.length() > MAX_TITLE)
	{
		std::wcsncpy(NotifyIconData.szInfoTitle, Title.c_str(), MAX_TITLE);
		NotifyIconData.szInfoTitle[MAX_TITLE] = wchar_t(0);
	}
	else
	{
		std::wcsncpy(NotifyIconData.szInfoTitle, Title.c_str(), Title.length() + 1);
	}

	if (Message.length() > MAX_MESSAGE)
	{
		std::wcsncpy(NotifyIconData.szInfo, Message.c_str(), MAX_MESSAGE);
		NotifyIconData.szInfo[MAX_MESSAGE] = wchar_t(0);
	}
	else
	{
		std::wcsncpy(NotifyIconData.szInfo, Message.c_str(), Message.length() + 1);
	}

	DWORD dwType;
	switch (IconType)
	{
	case BALLOON_ICON_TYPE::BIT_NONE:
		dwType = 0;
		break;
	case BALLOON_ICON_TYPE::BIT_INFO:
		dwType = NIIF_INFO;
		break;
	case BALLOON_ICON_TYPE::BIT_WARNING:
		dwType = NIIF_WARNING;
		break;
	case BALLOON_ICON_TYPE::BIT_ERROR:
		dwType = NIIF_ERROR;
		break;
	case BALLOON_ICON_TYPE::BIT_USER_DEFINED:
		dwType = NIIF_USER; // Use the "hBalloonIcon" parameter.
		break;
	}

	NotifyIconData.dwInfoFlags = dwType
		| (bSound ? 0 : NIIF_NOSOUND)
		| (bLargeIcon ? NIIF_LARGE_ICON : 0)
		| (bRespectQuiteTime ? NIIF_RESPECT_QUIET_TIME : 0);

	NotifyIconData.uTimeout = Timeout;
	NotifyIconData.hBalloonIcon = hBalloonIcon;
	NotifyIconData.uFlags = NIF_INFO | NIF_GUID;

	Update();
}

void SystemTrayIcon::Update()
{
	bLastResult = Shell_NotifyIconW(NIM_MODIFY, &NotifyIconData);
}