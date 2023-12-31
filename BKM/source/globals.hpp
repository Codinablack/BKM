#ifndef _GLOBALS_HPP
#define _GLOBALS_HPP

#include <string>
#include "enums.hpp"

namespace BKM {

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
}
#endif //_GLOBALS_HPP