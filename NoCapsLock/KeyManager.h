#pragma once
#include <map>
#include <windows.h>
#include "KeyObject.h"

class KeyManager
{
private:
	static std::map<DWORD_PTR, KeyObject::key_t> key_map;

public:
	static KeyObject::key_t FindKey(DWORD_PTR key);
	static KeyObject::key_t UpdateByKey(DWORD_PTR key, KeyObject::key_t newData);
	static bool Exist(DWORD_PTR key);
	static void AddKey(DWORD_PTR key, KeyObject::key_t value);
	static std::map<DWORD_PTR, KeyObject::key_t> GetKeyMap();
};

