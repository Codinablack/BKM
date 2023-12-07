#ifndef _SCRIPTMANAGER_H
#define _SCRIPTMANAGER_H
#include <unordered_map>
#include <map>
#include <lua.hpp>
#include <sol/sol.hpp>
#include "globals.h"
#include <mutex>
#include <filesystem>

class ScriptManager
{
public:
    void init();
	void loadScriptDirectory(const std::string& directory);
	bool loadConfigFile();
	void registerConfig();
	void createConfigTree();
	void registerUserTypes();
    const ConfigKey& getConfigKey(const std::string& name) const { return config_registry.at(name); }
	void setConfigKey(const std::string& key, const ConfigKey& value);
    static ScriptManager* getManager();
	void destroyManager();
	void deinit();

protected:
	ScriptManager() = default;
	~ScriptManager() = default;

private:
	static ScriptManager* sm_instance;
    static std::mutex sm_mutex;
    ScriptManager(ScriptManager& other) = delete;
    void operator=(const ScriptManager&) = delete;

	std::filesystem::path script_directory;
    std::unordered_map<std::string, ConfigKey> config_registry;
    std::map<std::string, std::string> sm_scripts;
	sol::state lua_man;
};

#endif