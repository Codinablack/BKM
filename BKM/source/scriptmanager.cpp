#include "scriptmanager.hpp"
#include <filesystem>
#include "item.hpp"
#include "thing.hpp"
#include "creature.hpp"
#include "player.hpp"
#include "monster.hpp"
#include "npc.hpp"
#include "console.hpp"


ScriptManager* ScriptManager::sm_instance = nullptr;
std::mutex ScriptManager::sm_mutex;

ScriptManager *ScriptManager::getManager() {
	// using mutex to make thread safe.
	std::lock_guard<std::mutex> lock(sm_mutex);
	if (sm_instance == nullptr)
	{
		sm_instance = new ScriptManager();
		// should possibly start init() here?
	}
	return sm_instance;
}

void ScriptManager::destroyManager() {
	shutdown();
	// should be unlock here I think. 
	std::lock_guard<std::mutex> unlock(sm_mutex);
	delete sm_instance;
	sm_instance = nullptr;
}

ScriptManager::~ScriptManager() {
	destroyManager();
}

void ScriptManager::init() {
	lua_man.open_libraries(sol::lib::base, sol::lib::package, sol::lib::os, sol::lib::table, sol::lib::string, sol::lib::io, sol::lib::debug, sol::lib::math);
	if (!loadConfigFile())
	{
		BK::Console::Print << "Failed to load config.lua" << "\n";
	}
	script_directory = std::filesystem::current_path();
	registerUserTypes();
	createConfigTree();
	registerConfig();
}

// TODO
// ensure all lua memory is really freed.
void ScriptManager::shutdown() {
	lua_man.clear_package_loaders();
	lua_man.collect_garbage();
	lua_man.~state();
}

void ScriptManager::loadScriptDirectory() {
    for (const auto& entry : std::filesystem::recursive_directory_iterator(script_directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".lua") {
			try
			{
				auto result = lua_man.safe_script_file(entry.path().string());
			}
			catch (const sol::error& e)
			{
				BK::Console::Print << "an unexpected error has occurred: " << e.what() << "\n";
			}
        }
    }
}

bool ScriptManager::loadConfigFile() {
	try
	{
		auto result = lua_man.safe_script_file("config.lua");
		return true;
	}
	catch (const sol::error& e)
	{
		BK::Console::Print << "Unable to load config.lua file make sure its in same folder as the server \n";
		BK::Console::Print << "The specific error thrown when loading config.lua is : " << e.what() << "\n";
		return false;
	}
}

void ScriptManager::createConfigTree() {
	// We create the config keys first with default values for safety.
	// Later we can overwrite any values we find in the config.lua file.

	setConfigKey("ServerName", ConfigKey(ConfigKeyType::string, "ServerName", "BlackMoon"));
	setConfigKey("ServerPort", ConfigKey(ConfigKeyType::integer, "ServerPort", "7212"));
	setConfigKey("ScriptDirectory", ConfigKey(ConfigKeyType::string, "ScriptDirectory", "/data"));
	setConfigKey("BindOnlyGlobalAddress", ConfigKey(ConfigKeyType::boolean, "BindOnlyGlobalAddress", "true"));
	setConfigKey("LoginProtocolPort", ConfigKey(ConfigKeyType::integer, "LoginProtocolPort", "7213"));
	setConfigKey("GameProtocolPort", ConfigKey(ConfigKeyType::integer, "GameProtocolPort", "7214"));
	setConfigKey("StatusProtocolPort", ConfigKey(ConfigKeyType::integer, "StatusProtocolPort", "7215"));
	setConfigKey("MaxPlayers", ConfigKey(ConfigKeyType::integer, "MaxPlayers", "1000"));
	setConfigKey("OnePlayerOnlinePerAccount", ConfigKey(ConfigKeyType::boolean, "OnePlayerOnlinePerAccount", "true"));
	setConfigKey("AllowMultiLogin", ConfigKey(ConfigKeyType::boolean, "AllowMultiLogin", "true"));
	setConfigKey("MaxSameIPConnection", ConfigKey(ConfigKeyType::integer, "MaxSameIPConnection", "5"));
	setConfigKey("MaxConnection", ConfigKey(ConfigKeyType::integer, "MaxConnection", "5000"));
	setConfigKey("MaxConnectionPerIP", ConfigKey(ConfigKeyType::integer, "MaxConnectionPerIP", "8000"));
	setConfigKey("MaxConnectionPerAccount", ConfigKey(ConfigKeyType::integer, "MaxConnectionPerAccount", "5000"));
	setConfigKey("MaxConnectionPerCharacter", ConfigKey(ConfigKeyType::integer, "MaxConnectionPerCharacter", "5000"));
}

void ScriptManager::registerConfig() {

	auto ServerName = lua_man.get<std::string>("ServerName");
	auto ServerPort = lua_man.get<int>("ServerPort");
	auto ScriptDirectory = lua_man.get<std::string>("ScriptDirectory");
	auto LoginProtocolPort = lua_man.get<int>("LoginProtocolPort");
	auto GameProtocolPort = lua_man.get<int>("GameProtocolPort");
	auto StatusProtocolPort = lua_man.get<int>("StatusProtocolPort");
	auto MaxPlayers = lua_man.get<int>("MaxPlayers");
	auto MaxSameIPConnection = lua_man.get<int>("MaxSameIPConnection");
	auto MaxConnection = lua_man.get<int>("MaxConnection");
	auto MaxConnectionsPerIP = lua_man.get<int>("MaxConnectionsPerIP");
	auto MaxConnectionsPerAccount = lua_man.get<int>("MaxConnectionsPerAccount");
	auto MaxConnectionsPerCharacter = lua_man.get<int>("MaxConnectionsPerCharacter");

	sol::optional<bool> BindOnlyGlobalAddress = lua_man.get<bool>("BindOnlyGlobalAddress");
	sol::optional<bool> OnePlayerOnlinePerAccount = lua_man.get<bool>("OnePlayerOnlinePerAccount");
	sol::optional<bool> AllowMultiLogin = lua_man.get<bool>("AllowMultiLogin");

	if (!ServerName.empty())
	{	// could possibly need further validation here and in all these if checks.
		setConfigKey("ServerName", ConfigKey(ConfigKeyType::string, "ServerName", ServerName));
	}

	if (ServerPort != 0)
	{
		setConfigKey("ServerPort", ConfigKey(ConfigKeyType::integer, "ServerPort", std::to_string(ServerPort)));
	}

	if (!ScriptDirectory.empty())
	{
		// MAY NEED FURTHER VALIDATION \\
		// we attach the relative path located in config for script directory
		// to the end of the initialized script_directory path (current dir).
		// then we set the slashes and backslashes to the current OS with make_preferred.
		setConfigKey("ScriptDirectory", ConfigKey(ConfigKeyType::string, "ScriptDirectory", ScriptDirectory));
		script_directory = script_directory / ScriptDirectory;
		script_directory.make_preferred();
	}

	if (BindOnlyGlobalAddress != std::nullopt)
	{
		setConfigKey("BindOnlyGlobalAddress", ConfigKey(ConfigKeyType::boolean, "BindOnlyGlobalAddress", *BindOnlyGlobalAddress ? "true" : "false"));
	}

	if (LoginProtocolPort != 0)
	{
		setConfigKey("LoginProtocolPort", ConfigKey(ConfigKeyType::integer, "LoginProtocolPort", std::to_string(LoginProtocolPort)));
	}

	if (GameProtocolPort != 0)
	{
		setConfigKey("GameProtocolPort", ConfigKey(ConfigKeyType::integer, "GameProtocolPort", std::to_string(GameProtocolPort)));
	}

	if (StatusProtocolPort != 0)
	{
		setConfigKey("StatusProtocolPort", ConfigKey(ConfigKeyType::integer, "StatusProtocolPort", std::to_string(StatusProtocolPort)));
	}

	if (MaxPlayers != 0)
	{
		setConfigKey("MaxPlayers", ConfigKey(ConfigKeyType::integer, "MaxPlayers", std::to_string(MaxPlayers)));
	}

	if (OnePlayerOnlinePerAccount != std::nullopt)
	{
		setConfigKey("OnePlayerOnlinePerAccount", ConfigKey(ConfigKeyType::boolean, "OnePlayerOnlinePerAccount", *OnePlayerOnlinePerAccount ? "true" : "false")); 
	}

	if (AllowMultiLogin != std::nullopt)
	{
		setConfigKey("AllowMultiLogin", ConfigKey(ConfigKeyType::boolean, "AllowMultiLogin", *AllowMultiLogin ? "true" : "false"));
	}

	if (MaxSameIPConnection != 0)
	{
		setConfigKey("MaxSameIPConnection", ConfigKey(ConfigKeyType::integer, "MaxSameIPConnection", std::to_string(MaxSameIPConnection)));
	}

	if (MaxConnection != 0)
	{
		setConfigKey("MaxConnection", ConfigKey(ConfigKeyType::integer, "MaxConnection", std::to_string(MaxConnection)));
	}

	if (MaxConnectionsPerIP != 0)
	{
		setConfigKey("MaxConnectionsPerIP", ConfigKey(ConfigKeyType::integer, "MaxConnectionsPerIP", std::to_string(MaxConnectionsPerIP)));
	}

	if (MaxConnectionsPerAccount != 0)
	{
		setConfigKey("MaxConnectionsPerAccount", ConfigKey(ConfigKeyType::integer, "MaxConnectionsPerAccount", std::to_string(MaxConnectionsPerAccount)));
	}

	if (MaxConnectionsPerCharacter != 0)
	{
		setConfigKey("MaxConnectionsPerCharacter", ConfigKey(ConfigKeyType::integer, "MaxConnectionsPerCharacter", std::to_string(MaxConnectionsPerCharacter)));
	}

	BK::Console::Print << "ServerName: " << getConfigKey("ServerName").value << "\n";
	BK::Console::Print << "ServerPort: " << getConfigKey("ServerPort").value << "\n";
	BK::Console::Print << "ScriptDirectory: " << getConfigKey("ScriptDirectory").value << "\n";
	BK::Console::Print << "BindOnlyGlobalAddress: " << getConfigKey("BindOnlyGlobalAddress").value << "\n";
	BK::Console::Print << "LoginProtocolPort: " << getConfigKey("LoginProtocolPort").value << "\n";
	BK::Console::Print << "GameProtocolPort: " << getConfigKey("GameProtocolPort").value << "\n";
	BK::Console::Print << "StatusProtocolPort: " << getConfigKey("StatusProtocolPort").value << "\n";
	BK::Console::Print << "MaxPlayers: " << getConfigKey("MaxPlayers").value << "\n";
	BK::Console::Print << "OnePlayerOnlinePerAccount: " << getConfigKey("OnePlayerOnlinePerAccount").value << "\n";
	BK::Console::Print << "AllowMultiLogin: " << getConfigKey("AllowMultiLogin").value << "\n";
	BK::Console::Print << "MaxSameIPConnection: " << getConfigKey("MaxSameIPConnection").value << "\n";
	BK::Console::Print << "MaxConnection: " << getConfigKey("MaxConnection").value << "\n";
	BK::Console::Print << "MaxConnectionsPerIP: " << getConfigKey("MaxConnectionsPerIP").value << "\n";
	BK::Console::Print << "MaxConnectionsPerAccount: " << getConfigKey("MaxConnectionsPerAccount").value << "\n";
	BK::Console::Print << "MaxConnectionsPerCharacter: " << getConfigKey("MaxConnectionsPerCharacter").value << "\n";
}

void ScriptManager::setConfigKey(const std::string& key, const ConfigKey& value)
{
	// this loop may be more expensive than necessary
	// but I want to make sure that the key is not already in the registry
	auto it = config_registry.find(key);
	if (it != config_registry.end()) {
		config_registry.erase(it);
	}
	config_registry[key] = value;
}


void ScriptManager::registerUserTypes() {
	/// constructorlist example.		sol::constructors<vector(),vector(float),void(float, float)>	clist;
	sol::usertype<Thing> thing_type = lua_man.new_usertype<Thing>("Thing", sol::call_constructor, sol::factories([]() {return new Thing(); }));

	sol::usertype<Creature> creature_type = lua_man.new_usertype<Creature>("Creature", sol::call_constructor, sol::factories([]() {return new Creature(); }));
	creature_type[sol::base_classes] = sol::bases<Thing>();

	sol::usertype<Player> player_type = lua_man.new_usertype<Player>("Player", sol::call_constructor, sol::factories([]() {return new Player(); }));
	player_type["getPlayer"] = &Player::getPlayer;
	player_type["getHealthPoints"] = &Player::getHealthPoints;
	player_type[sol::base_classes] = sol::bases<Creature>();

}