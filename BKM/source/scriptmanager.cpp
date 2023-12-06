#include "scriptmanager.h"
#include <filesystem>

ScriptManager* ScriptManager::sm_instance = nullptr;
std::mutex ScriptManager::sm_mutex;

ScriptManager *ScriptManager::getManager() {
	// using mutex to make thread safe.
	std::lock_guard<std::mutex> lock(sm_mutex);
	if (sm_instance == nullptr)
	{
		sm_instance = new ScriptManager();
	}
	return sm_instance;
}

void ScriptManager::destroyManager() {
	deinit();
	// should be unlock here I think. 
	std::lock_guard<std::mutex> unlock(sm_mutex);
	delete sm_instance;
	sm_instance = nullptr;
}

void ScriptManager::init() {
	lua_man.open_libraries(sol::lib::base, sol::lib::package, sol::lib::os, sol::lib::table, sol::lib::string, sol::lib::io, sol::lib::debug, sol::lib::math);
	if (!loadConfigFile())
	{
		std::cout << "Failed to load config.lua" << std::endl;
	}
	createConfigTree();
	registerConfig();
	registerUserTypes();
}

// TODO
// deregister user types
// deregister config
// destroy config tree
void ScriptManager::deinit() {
	lua_man.clear_package_loaders();
	lua_man.collect_garbage();
	lua_man.~state();
}

void ScriptManager::loadScriptDirectory(const std::string& directory) {
    for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".lua") {
			try
			{
				auto result = lua_man.safe_script_file(entry.path().string());
			}
			catch (const sol::error& e)
			{
				std::cout << "an unexpected error has occurred: " << e.what() << std::endl;
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
		std::cout << "Unable to load config.lua file make sure its in same folder as the server /n";
		std::cout << "The specific error thrown when loading config.lua is : " << e.what() << "/n";
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
			std::cout << "Current ServerName: " << getConfigKey("ServerName").value << std::endl;
			config_registry["ServerName"].value = ServerName;
			std::cout << "New ServerName: " << getConfigKey("ServerName").value << std::endl;
		}
		else
		{
			throw std::runtime_error("ServerName not found in config.lua");
		}
	}
	catch (const sol::error& e)
	{
		std::cout << "an expected error has occurred: " << e.what() << std::endl;
	}

	try
	{
		auto ServerPort = lua_man.get<double>("ServerPort");
		if (ServerPort != 0)
		{
			std::cout << "Current ServerPort: " << getConfigKey("ServerPort").value << std::endl;
			config_registry["ServerPort"].value = std::to_string(ServerPort);
			std::cout << "ServerPort: " << getConfigKey("ServerPort").value << std::endl;
		}
		else
		{
			throw std::runtime_error("ServerPort not found in config.lua");
		}
	}
	catch (const sol::error& e)
	{
		std::cout << "an expected error has occurred: " << e.what() << std::endl;
	}

	try
	{
		auto ScriptDirectory = lua_man.get<std::string>("ScriptDirectory");
		if (!ScriptDirectory.empty())
		{
			config_registry["ScriptDirectory"].value = ScriptDirectory;
			std::cout << "ScriptDirectory: " << ScriptDirectory << std::endl;
		}
		else
		{
			throw std::runtime_error("ScriptDirectory not found in config.lua");
		}
	}
	catch (const sol::error& e)
	{
		std::cout << "an expected error has occurred: " << e.what() << std::endl;
	}

	try
	{
		std::optional<bool> BindOnlyGlobalAddress = lua_man.get<bool>("BindOnlyGlobalAddress");
		if (BindOnlyGlobalAddress != std::nullopt)
		{
			config_registry["BindOnlyGlobalAddress"].value = *BindOnlyGlobalAddress;
			std::cout << "BindOnlyGlobalAddress: " << *BindOnlyGlobalAddress << std::endl;
		}
		else
		{
			throw std::runtime_error("BindOnlyGlobalAddress not found in config.lua");
		}
	}
	catch (const sol::error& e)
	{
		std::cout << "an expected error has occurred: " << e.what() << std::endl;
	}

	try
	{
		auto LoginProtocolPort = lua_man.get<double>("LoginProtocolPort");
		if (LoginProtocolPort != 0)
		{
			config_registry["LoginProtocolPort"].value = std::to_string(LoginProtocolPort);
			std::cout << "LoginProtocolPort: " << LoginProtocolPort << std::endl;
		}
		else
		{
			throw std::runtime_error("LoginProtocolPort not found in config.lua");
		}
	}
	catch (const sol::error& e)
	{
		std::cout << "an expected error has occurred: " << e.what() << std::endl;
	}

	try
	{
		auto GameProtocolPort = lua_man.get<double>("GameProtocolPort");
		if (GameProtocolPort != 0)
		{
			config_registry["GameProtocolPort"].value = std::to_string(GameProtocolPort);
			std::cout << "GameProtocolPort: " << GameProtocolPort << std::endl;
		}
		else
		{
			throw std::runtime_error("GameProtocolPort not found in config.lua");
		}
	}
	catch (const sol::error& e)
	{
		std::cout << "an expected error has occurred: " << e.what() << std::endl;
	}

	try
	{
		auto StatusProtocolPort = lua_man.get<double>("StatusProtocolPort");
		if (StatusProtocolPort != 0)
		{
			config_registry["StatusProtocolPort"].value = std::to_string(StatusProtocolPort);
			std::cout << "StatusProtocolPort: " << StatusProtocolPort << std::endl;
		}
		else
		{
			throw std::runtime_error("StatusProtocolPort not found in config.lua");
		}
	}
	catch (const sol::error& e)
	{
		std::cout << "an expected error has occurred: " << e.what() << std::endl;
	}

	try
	{
		auto MaxPlayers = lua_man.get<double>("MaxPlayers");
		if (MaxPlayers != 0)
		{
			config_registry["MaxPlayers"].value = std::to_string(MaxPlayers);
			std::cout << "MaxPlayers: " << MaxPlayers << std::endl;
		}
		else
		{
			throw std::runtime_error("MaxPlayers not found in config.lua");
		}
	}
	catch (const sol::error& e)
	{
		std::cout << "an expected error has occurred: " << e.what() << std::endl;
	}

	try
	{
		std::optional<bool> OnePlayerOnlinePerAccount = lua_man.get<bool>("OnePlayerOnlinePerAccount");
		if (OnePlayerOnlinePerAccount != std::nullopt)
		{
			config_registry["OnePlayerOnlinePerAccount"].value = *OnePlayerOnlinePerAccount;
			std::cout << "OnePlayerOnlinePerAccount: " << *OnePlayerOnlinePerAccount << std::endl;
		}
		else
		{
			throw std::runtime_error("OnePlayerOnlinePerAccount not found in config.lua");
		}
	}
	catch (const sol::error& e)
	{
		std::cout << "an expected error has occurred: " << e.what() << std::endl;
	}

	try
	{
		std::optional<bool> AllowMultiLogin = lua_man.get<bool>("AllowMultiLogin");
		if (AllowMultiLogin != std::nullopt)
		{
			config_registry["AllowMultiLogin"].value = *AllowMultiLogin;
			std::cout << "AllowMultiLogin: " << *AllowMultiLogin << std::endl;
		}
		else
		{
			throw std::runtime_error("AllowMultiLogin not found in config.lua");
		}
	}
	catch (const sol::error& e)
	{
		std::cout << "an expected error has occurred: " << e.what() << std::endl;
	}

	try
	{
		auto MaxSameIPConnection = lua_man.get<double>("MaxSameIPConnection");
		if (MaxSameIPConnection != 0)
		{
			config_registry["MaxSameIPConnection"].value = std::to_string(MaxSameIPConnection);
			std::cout << "MaxSameIPConnection: " << MaxSameIPConnection << std::endl;
		}
		else
		{
			throw std::runtime_error("MaxSameIPConnection not found in config.lua");
		}
	}
	catch (const sol::error& e)
	{
		std::cout << "an expected error has occurred: " << e.what() << std::endl;
	}

	try
	{
		auto MaxConnection = lua_man.get<double>("MaxConnection");
		if (MaxConnection != 0)
		{
			config_registry["MaxConnection"].value = std::to_string(MaxConnection);
			std::cout << "MaxConnection: " << MaxConnection << std::endl;
		}
		else
		{
			throw std::runtime_error("MaxConnection not found in config.lua");
		}
	}
	catch (const sol::error& e)
	{
		std::cout << "an expected error has occurred: " << e.what() << std::endl;
	}

	try
	{

		auto MaxConnectionsPerIP = lua_man.get<double>("MaxConnectionsPerIP");
		if (MaxConnectionsPerIP != 0)
		{
			config_registry["MaxConnectionsPerIP"].value = std::to_string(MaxConnectionsPerIP);
			std::cout << "MaxConnectionsPerIP: " << MaxConnectionsPerIP << std::endl;
		}
		else
		{
			throw std::runtime_error("MaxConnectionsPerIP not found in config.lua");
		}
	}
	catch (const sol::error& e)
	{
		std::cout << "an expected error has occurred: " << e.what() << std::endl;
	}

	try
	{
		auto MaxConnectionsPerAccount = lua_man.get<double>("MaxConnectionsPerAccount");
		if (MaxConnectionsPerAccount != 0)
		{
			config_registry["MaxConnectionsPerAccount"].value = std::to_string(MaxConnectionsPerAccount);
			std::cout << "MaxConnectionsPerAccount: " << MaxConnectionsPerAccount << std::endl;
		}
		else
		{
			throw std::runtime_error("MaxConnectionsPerAccount not found in config.lua");
		}
	}
	catch (const sol::error& e)
	{
		std::cout << "an expected error has occurred: " << e.what() << std::endl;
	}

	try
	{
		auto MaxConnectionsPerCharacter = lua_man.get<double>("MaxConnectionsPerCharacter");
		if (MaxConnectionsPerCharacter != 0)
		{
			config_registry["MaxConnectionsPerCharacter"].value = std::to_string(MaxConnectionsPerCharacter);
			std::cout << "MaxConnectionsPerCharacter: " << MaxConnectionsPerCharacter << std::endl;
		}
		else
		{
			throw std::runtime_error("MaxConnectionsPerCharacter not found in config.lua");
		}
	}
	catch (const sol::error& e)
	{
		std::cout << "an expected error has occurred: " << e.what() << std::endl;
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
	//lua_man.new_usertype<player>("player",
	//	"shoot", &player::shoot,
	//	"boost", &player::boost,
	//	"set_hp", &player::set_hp,
	//	"get_hp", &player::get_hp
	//);
}