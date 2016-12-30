#include "ContextMenuHandler.h"

ContextMenuHandler::ContextMenuHandler(HWND parent)
{
	_parent = parent;
	_hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU1));
}


ContextMenuHandler::~ContextMenuHandler()
{
}

void ContextMenuHandler::CreateMenu() {
	if (_created) return;

	std::map<DWORD_PTR, KeyObject::key_t> keys = KeyManager::GetKeyMap();
	std::map<DWORD_PTR, KeyObject::key_t>::iterator it;
	for (it = keys.begin(); it != keys.end(); it++)
	{
		//InsertMenu(_hMenu, 0, MFT_STRING, it->first, it->second.title.c_str());
		AddItem(0, MFT_STRING, it->first, it->second.title.c_str());
		CheckItem(static_cast<int>(it->first), it->second.enabled ? MF_CHECKED : MF_UNCHECKED);
	}

	AddItem(0, MFT_SEPARATOR, -1, "-");
	AddItem(0, MFT_STRING, ID__SHOWOPTIONS, helpers::GetString(IDS_SHOWOPTIONS));

#if _DEBUG
	AddItem(0, MFT_SEPARATOR, -1, "-");
	AddItem(0, MFT_STRING, 0x9293, helpers::GetString(IDS_SHOWCONSOLE));
#endif

	_created = true;
}

void ContextMenuHandler::AddItem(UINT pos, UINT type, UINT_PTR uIDNewItem, const char * text) {
	InsertMenu(_hMenu, pos, type, uIDNewItem, text);
}

void ContextMenuHandler::CheckItem(UINT id, bool checked) {
	CheckMenuItem(_hMenu, id, checked ? MF_CHECKED : MF_UNCHECKED);
}

void ContextMenuHandler::Show(POINT pos) {
	if (!_created) return;

	SetForegroundWindow(_parent);
	TrackPopupMenu((HMENU)GetSubMenu(_hMenu, 0), TPM_LEFTALIGN, pos.x, pos.y, 0, _parent, NULL);
}
