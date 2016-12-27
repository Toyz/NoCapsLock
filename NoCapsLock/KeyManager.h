#pragma once
#include <map>
#include <windows.h>
#include "KeyObject.h"

class KeyManager
{
private:
	static std::map<DWORD, KeyObject::key_t> key_map;

public:
	static KeyObject::key_t FindKey(DWORD key);
	static KeyObject::key_t UpdateByKey(DWORD key, KeyObject::key_t newData);
	static KeyObject::key_t UpdateByValue(UINT key, KeyObject::key_t newData);
	static KeyObject::key_t GetByValue(UINT key);
	static bool Exist(DWORD key);
	static void AddKey(DWORD key, KeyObject::key_t value);
	static std::map<DWORD, KeyObject::key_t> GetKeyMap();
};

