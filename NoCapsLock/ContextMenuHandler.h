#pragma once
#include <windows.h>
#include <VersionHelpers.h>
#include <stdio.h>
#include <string.h>

#include "resource.h"
#include "helpers.h"
#include "KeyManager.h"
#include "KeyObject.h"

class ContextMenuHandler
{
private:
	bool _created;
	HMENU _hMenu;
	HWND _parent;

public:
	ContextMenuHandler() { }
	ContextMenuHandler(HWND parent);
	~ContextMenuHandler();

	void CreateMenu();
	void AddItem(UINT Pos, UINT type, UINT_PTR uIDNewItem, const char * text);
	void CheckItem(UINT id, bool checked);

	void Show(POINT pos);
};

