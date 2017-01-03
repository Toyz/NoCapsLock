#include "ConfigHandler.h"

ConfigHandler::ConfigHandler(std::string configFile) {
	_configFile = configFile;
}

ConfigHandler::~ConfigHandler() {
}

void ConfigHandler::Load() {
	std::ifstream myfile;
	myfile.open(_configFile);

	std::string line;
	while (std::getline(myfile, line))
	{
		if (std::empty(line)) {
			_confHeader += "\n";
			continue;
		}

		std::vector<std::string> values;
		int test = helpers::split(line, values, '|');

		if (line.at(0) == '#' || test <= 0 || values.size() <= 0) {
			_confHeader += line + "\n";
			continue;
		}
		int key = std::stoi(values[0]);
		bool enabled = helpers::StringToBool(values[1]);
		std::string title = values[2];

		KeyManager::Add(key, KeyObject::CreateKey(title, enabled));
	}

	myfile.close();
}

void ConfigHandler::Save() {
	std::ofstream myfile;
	myfile.open(_configFile);

	std::map<DWORD_PTR, KeyObject::key_t> keys = KeyManager::AllKeys();
	std::map<DWORD_PTR, KeyObject::key_t>::iterator it;
	myfile << _confHeader;

	for (it = keys.begin(); it != keys.end(); it++)
	{
		myfile << it->first << "|" << it->second.enabled << "|" << it->second.title << "\n";
	}
	myfile.close();
}
