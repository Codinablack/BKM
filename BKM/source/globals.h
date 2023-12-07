#ifndef _GLOBALS_H
#define _GLOBALS_H

#include <string>
#include "enums.h"

struct ConfigKey
{
	ConfigKey() { type = ConfigKeyType::none; };
	ConfigKey(ConfigKeyType type, std::string name) : type(type), name(name) {};
	ConfigKey(ConfigKeyType type, std::string name, std::string value) : type(type), name(name), value(value) {};
	~ConfigKey() = default;
	ConfigKeyType type;
	std::string name = "";
	std::string value = "";
};

#endif