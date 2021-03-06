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
	_created = true;

	for (std::pair<DWORD_PTR, KeyObject::key_t> it : KeyManager::AllKeys())
	{
		AddItem(0, MFT_STRING, it.first, it.second.title.c_str());
		CheckItem(static_cast<int>(it.first), it.second.enabled);
	}

	AddItem(0, MFT_SEPARATOR, -1, "-");
	AddItem(0, MFT_STRING, ID__SHOWOPTIONS, helpers::GetString(IDS_SHOWOPTIONS));

#if _DEBUG
	AddItem(0, MFT_SEPARATOR, -1, "-");
	AddItem(0, MFT_STRING, 0x9293, helpers::GetString(IDS_SHOWCONSOLE));
#endif

}

void ContextMenuHandler::AddItem(UINT pos, UINT type, UINT_PTR uIDNewItem, const char * text) {
	if (!_created) return;
	InsertMenu(_hMenu, pos, type, uIDNewItem, text);
}

void ContextMenuHandler::CheckItem(UINT id, bool checked) {
	if (!_created) return;
	CheckMenuItem(_hMenu, id, checked ? MF_CHECKED : MF_UNCHECKED);
}

void ContextMenuHandler::Show(POINT pos) {
	if (!_created) return;

	SetForegroundWindow(_parent);
	TrackPopupMenu((HMENU)GetSubMenu(_hMenu, 0), TPM_LEFTALIGN, pos.x, pos.y, 0, _parent, NULL);
}
