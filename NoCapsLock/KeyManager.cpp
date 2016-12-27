#include "KeyManager.h"

std::map<DWORD, KeyObject::key_t> KeyManager::key_map;

KeyObject::key_t KeyManager::FindKey(DWORD key) {
	if (Exist(key)) {
		return key_map[key];
	}

	return { 0 };
}

KeyObject::key_t KeyManager::UpdateByValue(UINT key, KeyObject::key_t newData) {
	std::map<DWORD, KeyObject::key_t>::iterator it;

	DWORD id = -1;
	for (it = key_map.begin(); it != key_map.end(); it++)
	{
		if (it->second.MenuID == key) {
			id = it->first;
			break;
		}
	}

	key_map[id] = newData;

	return newData;
}

KeyObject::key_t KeyManager::GetByValue(UINT key) {
	std::map<DWORD, KeyObject::key_t>::iterator it;

	DWORD id = -1;
	for (it = key_map.begin(); it != key_map.end(); it++)
	{
		if (it->second.MenuID == key) {
			return it->second;
		}
	}

	return { 0 };
}

void KeyManager::AddKey(DWORD key, KeyObject::key_t value) {
	if (Exist(key)) return;

	key_map[key] = value;
}

bool KeyManager::Exist(DWORD key) {
	std::map<DWORD, KeyObject::key_t>::iterator checking = key_map.find(key);

	return checking != key_map.end();
}

std::map<DWORD, KeyObject::key_t> KeyManager::GetKeyMap() {
	return key_map;
}



