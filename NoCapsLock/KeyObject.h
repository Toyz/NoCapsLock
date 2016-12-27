#pragma once
#include <string>

class KeyObject
{
public:
	struct key_t
	{
		std::string title;
		bool enabled;
	};

public:
	static key_t CreateKey(std::string title) {
		return CreateKey(title, false);
	}

	static key_t CreateKey(std::string title, bool enabled) {
		return { title, enabled };
	}
};

