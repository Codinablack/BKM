#ifndef _SCRIPTMANAGER_HPP
#define _SCRIPTMANAGER_HPP
#include <unordered_map>
#include <map>
#include <lua.hpp>
#include <sol/sol.hpp>
#include "globals.hpp"
#include <mutex>
#include <filesystem>

namespace BKM {

	class ScriptManager
	{
	public:
		void init();
		void loadScriptDirectory(std::filesystem::path filepath);
		bool loadConfigFile();
		void registerConfig();
		void createConfigTree();
		void registerUserTypes();
		const ConfigKey& getConfigKey(const std::string& name) const { return config_registry.at(name); }
		void setConfigKey(const std::string& key, const ConfigKey& value);
		void shutdown();
		ScriptManager();
		~ScriptManager();

	private:
		ScriptManager(ScriptManager& other) = delete;
		void operator=(const ScriptManager&) = delete;

		std::filesystem::path script_directory;
		std::unordered_map<std::string, ConfigKey> config_registry;
		sol::state lua_man;
	};
}// BKM
#endif