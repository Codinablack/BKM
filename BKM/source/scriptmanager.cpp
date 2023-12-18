#include "scriptmanager.hpp"
#include <filesystem>
#include "item.hpp"
#include "thing.hpp"
#include "creature.hpp"
#include "player.hpp"
#include "monster.hpp"
#include "npc.hpp"


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
		std::cout << "Failed to load config.lua" << "\n";
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
				std::cout << "an unexpected error has occurred: " << e.what() << "\n";
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
		std::cout << "Unable to load config.lua file make sure its in same folder as the server \n";
		std::cout << "The specific error thrown when loading config.lua is : " << e.what() << "\n";
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
	try
	{
		auto ServerName = lua_man.get<std::string>("ServerName");
		if (!ServerName.empty())
		{	// could possibly need further validation here
			std::cout << "Current ServerName: " << getConfigKey("ServerName").value << "\n";
			setConfigKey("ServerName", ConfigKey(ConfigKeyType::string, "ServerName", ServerName));
			std::cout << "New ServerName: " << getConfigKey("ServerName").value << "\n";
		}
		else
		{
			throw std::runtime_error("ServerName not found in config.lua");
		}
	}
	catch (const sol::error& e)
	{
		std::cout << "an expected error has occurred: " << e.what() << "\n";
	}

	try
	{
		auto ServerPort = lua_man.get<int>("ServerPort");
		if (ServerPort != 0)
		{
			std::cout << "Current ServerPort: " << getConfigKey("ServerPort").value << "\n";
			setConfigKey("ServerPort", ConfigKey(ConfigKeyType::integer, "ServerPort", std::to_string(ServerPort)));
			std::cout << "ServerPort: " << getConfigKey("ServerPort").value << "\n";
		}
		else
		{
			throw std::runtime_error("ServerPort not found in config.lua");
		}
	}
	catch (const sol::error& e)
	{
		std::cout << "an expected error has occurred: " << e.what() << "\n";
	}

	try
	{
		auto ScriptDirectory = lua_man.get<std::string>("ScriptDirectory");
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
		else
		{
			throw std::runtime_error("ScriptDirectory not found in config.lua");
		}
	}
	catch (const sol::error& e)
	{
		std::cout << "an expected error has occurred: " << e.what() << "\n";
	}

	try
	{
		sol::optional<bool> BindOnlyGlobalAddress = lua_man.get<bool>("BindOnlyGlobalAddress");
		if (BindOnlyGlobalAddress != std::nullopt)
		{
			config_registry["BindOnlyGlobalAddress"].value = *BindOnlyGlobalAddress;
			setConfigKey("BindOnlyGlobalAddress", ConfigKey(ConfigKeyType::boolean, "BindOnlyGlobalAddress", *BindOnlyGlobalAddress ? "true" : "false"));
			std::cout << "BindOnlyGlobalAddress: " << getConfigKey("BindOnlyGlobalAddress").value << "\n";
		}
		else
		{
			throw std::runtime_error("BindOnlyGlobalAddress not found in config.lua");
		}
	}
	catch (const sol::error& e)
	{
		std::cout << "an expected error has occurred: " << e.what() << "\n";
	}

	try
	{
		auto LoginProtocolPort = lua_man.get<int>("LoginProtocolPort");
		if (LoginProtocolPort != 0)
		{
			setConfigKey("LoginProtocolPort", ConfigKey(ConfigKeyType::integer, "LoginProtocolPort", std::to_string(LoginProtocolPort)));
			std::cout << "LoginProtocolPort: " << getConfigKey("LoginProtocolPort").value << "\n";
		}
		else
		{
			throw std::runtime_error("LoginProtocolPort not found in config.lua");
		}
	}
	catch (const sol::error& e)
	{
		std::cout << "an expected error has occurred: " << e.what() << "\n";
	}

	try
	{
		auto GameProtocolPort = lua_man.get<int>("GameProtocolPort");
		if (GameProtocolPort != 0)
		{
			setConfigKey("GameProtocolPort", ConfigKey(ConfigKeyType::integer, "GameProtocolPort", std::to_string(GameProtocolPort)));
			std::cout << "GameProtocolPort: " << getConfigKey("GameProtocolPort").value << "\n";
		}
		else
		{
			throw std::runtime_error("GameProtocolPort not found in config.lua");
		}
	}
	catch (const sol::error& e)
	{
		std::cout << "an expected error has occurred: " << e.what() << "\n";
	}

	try
	{
		auto StatusProtocolPort = lua_man.get<int>("StatusProtocolPort");
		if (StatusProtocolPort != 0)
		{
			setConfigKey("StatusProtocolPort", ConfigKey(ConfigKeyType::integer, "StatusProtocolPort", std::to_string(StatusProtocolPort)));
			std::cout << "StatusProtocolPort: " << getConfigKey("StatusProtocolPort").value << "\n";
		}
		else
		{
			throw std::runtime_error("StatusProtocolPort not found in config.lua");
		}
	}
	catch (const sol::error& e)
	{
		std::cout << "an expected error has occurred: " << e.what() << "\n";
	}

	try
	{
		auto MaxPlayers = lua_man.get<int>("MaxPlayers");
		if (MaxPlayers != 0)
		{
			setConfigKey("MaxPlayers", ConfigKey(ConfigKeyType::integer, "MaxPlayers", std::to_string(MaxPlayers)));
			std::cout << "MaxPlayers: " << getConfigKey("MaxPlayers").value << "\n";
		}
		else
		{
			throw std::runtime_error("MaxPlayers not found in config.lua");
		}
	}
	catch (const sol::error& e)
	{
		std::cout << "an expected error has occurred: " << e.what() << "\n";
	}

	try
	{
		sol::optional<bool> OnePlayerOnlinePerAccount = lua_man.get<bool>("OnePlayerOnlinePerAccount");
		if (OnePlayerOnlinePerAccount != std::nullopt)
		{
			setConfigKey("OnePlayerOnlinePerAccount", ConfigKey(ConfigKeyType::boolean, "OnePlayerOnlinePerAccount", *OnePlayerOnlinePerAccount ? "true" : "false")); 
			std::cout << "OnePlayerOnlinePerAccount: " << getConfigKey("OnePlayerOnlinePerAccount").value << "\n";
		}
		else
		{
			throw std::runtime_error("OnePlayerOnlinePerAccount not found in config.lua");
		}
	}
	catch (const sol::error& e)
	{
		std::cout << "an expected error has occurred: " << e.what() << "\n";
	}

	try
	{
		sol::optional<bool> AllowMultiLogin = lua_man.get<bool>("AllowMultiLogin");
		if (AllowMultiLogin != std::nullopt)
		{
			setConfigKey("AllowMultiLogin", ConfigKey(ConfigKeyType::boolean, "AllowMultiLogin", *AllowMultiLogin ? "true" : "false"));
			std::cout << "AllowMultiLogin: " << getConfigKey("AllowMultiLogin").value << "\n";
		}
		else
		{
			throw std::runtime_error("AllowMultiLogin not found in config.lua");
		}
	}
	catch (const sol::error& e)
	{
		std::cout << "an expected error has occurred: " << e.what() << "\n";
	}

	try
	{
		auto MaxSameIPConnection = lua_man.get<int>("MaxSameIPConnection");
		if (MaxSameIPConnection != 0)
		{
			setConfigKey("MaxSameIPConnection", ConfigKey(ConfigKeyType::integer, "MaxSameIPConnection", std::to_string(MaxSameIPConnection)));
			std::cout << "MaxSameIPConnection: " << getConfigKey("MaxSameIPConnection").value << "\n";
		}
		else
		{
			throw std::runtime_error("MaxSameIPConnection not found in config.lua");
		}
	}
	catch (const sol::error& e)
	{
		std::cout << "an expected error has occurred: " << e.what() << "\n";
	}

	try
	{
		auto MaxConnection = lua_man.get<int>("MaxConnection");
		if (MaxConnection != 0)
		{
			setConfigKey("MaxConnection", ConfigKey(ConfigKeyType::integer, "MaxConnection", std::to_string(MaxConnection)));
			std::cout << "MaxConnection: " << getConfigKey("MaxConnection").value << "\n";
		}
		else
		{
			throw std::runtime_error("MaxConnection not found in config.lua");
		}
	}
	catch (const sol::error& e)
	{
		std::cout << "an expected error has occurred: " << e.what() << "\n";
	}

	try
	{

		auto MaxConnectionsPerIP = lua_man.get<int>("MaxConnectionsPerIP");
		if (MaxConnectionsPerIP != 0)
		{
			setConfigKey("MaxConnectionsPerIP", ConfigKey(ConfigKeyType::integer, "MaxConnectionsPerIP", std::to_string(MaxConnectionsPerIP)));
			std::cout << "MaxConnectionsPerIP: " << getConfigKey("MaxConnectionsPerIP").value << "\n";
		}
		else
		{
			throw std::runtime_error("MaxConnectionsPerIP not found in config.lua");
		}
	}
	catch (const sol::error& e)
	{
		std::cout << "an expected error has occurred: " << e.what() << "\n";
	}

	try
	{
		auto MaxConnectionsPerAccount = lua_man.get<int>("MaxConnectionsPerAccount");
		if (MaxConnectionsPerAccount != 0)
		{
			setConfigKey("MaxConnectionsPerAccount", ConfigKey(ConfigKeyType::integer, "MaxConnectionsPerAccount", std::to_string(MaxConnectionsPerAccount)));
			std::cout << "MaxConnectionsPerAccount: " << getConfigKey("MaxConnectionsPerAccount").value << "\n";
		}
		else
		{
			throw std::runtime_error("MaxConnectionsPerAccount not found in config.lua");
		}
	}
	catch (const sol::error& e)
	{
		std::cout << "an expected error has occurred: " << e.what() << "\n";
	}

	try
	{
		auto MaxConnectionsPerCharacter = lua_man.get<int>("MaxConnectionsPerCharacter");
		if (MaxConnectionsPerCharacter != 0)
		{
			setConfigKey("MaxConnectionsPerCharacter", ConfigKey(ConfigKeyType::integer, "MaxConnectionsPerCharacter", std::to_string(MaxConnectionsPerCharacter)));
			std::cout << "MaxConnectionsPerCharacter: " << getConfigKey("MaxConnectionsPerCharacter").value << "\n";
		}
		else
		{
			throw std::runtime_error("MaxConnectionsPerCharacter not found in config.lua");
		}
	}
	catch (const sol::error& e)
	{
		std::cout << "an expected error has occurred: " << e.what() << "\n";
	}
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
	thing_type["getItem"] = &Thing::getItem;
	thing_type["getCreature"] = &Thing::getCreature;

	sol::usertype<Creature> creature_type = lua_man.new_usertype<Creature>("Creature", sol::constructors<Creature(sol::this_state)>());
	creature_type["getCreature"] = &Creature::getCreature;
	creature_type["getPlayer"] = &Creature::getPlayer;
	creature_type[sol::base_classes] = sol::bases<Thing>();

	sol::usertype<Player> player_type = lua_man.new_usertype<Player>("Player", sol::constructors<Player(sol::this_state)>());
	player_type["getPlayer"] = &Player::getPlayer;
	player_type["getMonster"] = &Player::getMonster;
	player_type["getHealthPoints"] = &Player::getHealthPoints;
	player_type[sol::base_classes] = sol::bases<Creature>();

}