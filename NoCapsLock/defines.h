#pragma once
#define WM_CONTEXTMSGEVENT (WM_USER + 1)
#define NOCAPSCLASS "NoCapsLockWindow"

#if _DEBUG
#define SHOW_WINDOW 1
#else
#define SHOW_WINDOW 0
#endif

#define ID__BLOCKCAPSLOCK               40004
#define ID__DISABLEWINDOWSKEYL          40006
#define ID__DISABLEAPPSKEY              40007
#define ID__DISABLEWINDOWSKEYR          40008