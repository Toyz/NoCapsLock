#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "helpers.h"
#include "KeyManager.h"
#include "KeyObject.h"

class ConfigHandler
{
private:
	std::string _configFile;
	std::string _confHeader;

public:
	ConfigHandler() { } 
	ConfigHandler(std::string configFile);
	~ConfigHandler();

	void Load();
	void Save();
};

