#pragma once
#include <windows.h>
#include <VersionHelpers.h>
#include <stdio.h>
#include <string.h>

#include "resource.h"
#include "helpers.h"

class TaskbarNotifierHandler
{
private:
	HWND _parent;
	bool _isCreated;
	NOTIFYICONDATA Tray;

public:
	TaskbarNotifierHandler() { }
	TaskbarNotifierHandler(HWND parent);
	~TaskbarNotifierHandler();
	void Create();
	void ChangeTitle(const char * title);
};

