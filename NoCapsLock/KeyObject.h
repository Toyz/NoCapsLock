#pragma once
#include <string>

class KeyObject
{
public:
	struct key_t
	{
		UINT MenuID;
		std::string title;
		bool enabled;
	};

public:
	static key_t CreateKey(UINT id, std::string title) {
		return CreateKey(id, title, false);
	}

	static key_t CreateKey(UINT id, std::string title, bool enabled) {
		return { id, title, enabled };
	}
};

