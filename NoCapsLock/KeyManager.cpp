#include "KeyManager.h"

std::map<DWORD_PTR, KeyObject::key_t> KeyManager::key_map;

KeyObject::key_t KeyManager::Find(DWORD_PTR key) {
	if (Exist(key)) {
		return key_map[key];
	}

	return { "", false };
}

KeyObject::key_t KeyManager::Update(DWORD_PTR key, KeyObject::key_t newData) {
	if (Exist(key)) {
		key_map[key] = newData;
		return newData;
	}

	return{ "", false };
}

void KeyManager::Add(DWORD_PTR key, KeyObject::key_t value) {
	if (Exist(key)) return;

	key_map[key] = value;
}

bool KeyManager::Exist(DWORD_PTR key) {
	std::map<DWORD_PTR, KeyObject::key_t>::iterator checking = key_map.find(key);

	return checking != key_map.end();
}

std::map<DWORD_PTR, KeyObject::key_t> KeyManager::AllKeys() {
	return key_map;
}



