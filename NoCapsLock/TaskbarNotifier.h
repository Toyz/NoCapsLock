#pragma once
#include <windows.h>
#include <VersionHelpers.h>
#include <stdio.h>
#include <string.h>

#include "resource.h"
#include "helpers.h"

class TaskbarNotifier
{
private:
	HWND _parent;
	bool _isCreated;
	NOTIFYICONDATA Tray;

public:
	TaskbarNotifier() { }
	TaskbarNotifier(HWND parent);
	~TaskbarNotifier();
	void Create();
	void ChangeTitle(const char * title);
};

