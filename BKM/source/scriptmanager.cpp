#include "scriptmanager.hpp"
#include <filesystem>
#include "item.hpp"
#include "thing.hpp"
#include "creature.hpp"
#include "player.hpp"
#include "monster.hpp"
#include "npc.hpp"
#include "console.hpp"
#include "vocation.hpp"

namespace BKM {

	ScriptManager::ScriptManager() {
		init();
	}

	ScriptManager::~ScriptManager() {
		shutdown();
	}

	void ScriptManager::init() {
		lua_man.open_libraries(sol::lib::base, sol::lib::package, sol::lib::os, sol::lib::table, sol::lib::string, sol::lib::io, sol::lib::debug, sol::lib::math);
		if (!loadConfigFile())
		{
			Console::Print << "Failed to load config.lua" << "\n";
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

	void ScriptManager::loadScriptDirectory(std::filesystem::path directory) {
		for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
			if (entry.is_regular_file() && entry.path().extension() == ".lua") {
				try
				{
					auto result = lua_man.safe_script_file(entry.path().string());
				}
				catch (const sol::error& e)
				{
					Console::Print << "an unexpected error has occurred: " << e.what() << "\n";
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
			Console::Print << "Unable to load config.lua file make sure its in same folder as the server \n";
			Console::Print << "The specific error thrown when loading config.lua is : " << e.what() << "\n";
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

		auto ServerName = lua_man.get<std::string_view>("ServerName");
		auto ServerPort = lua_man.get<int>("ServerPort");
		auto ScriptDirectory = lua_man.get<std::string_view>("ScriptDirectory");
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
			setConfigKey("ServerName", ConfigKey(ConfigKeyType::string, "ServerName", static_cast<std::string>(ServerName)));
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
			setConfigKey("ScriptDirectory", ConfigKey(ConfigKeyType::string, "ScriptDirectory", static_cast<std::string>(ScriptDirectory)));
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

		Console::Print << "ServerName: " << getConfigKey("ServerName").value << "\n";
		Console::Print << "ServerPort: " << getConfigKey("ServerPort").value << "\n";
		Console::Print << "ScriptDirectory: " << getConfigKey("ScriptDirectory").value << "\n";
		Console::Print << "BindOnlyGlobalAddress: " << getConfigKey("BindOnlyGlobalAddress").value << "\n";
		Console::Print << "LoginProtocolPort: " << getConfigKey("LoginProtocolPort").value << "\n";
		Console::Print << "GameProtocolPort: " << getConfigKey("GameProtocolPort").value << "\n";
		Console::Print << "StatusProtocolPort: " << getConfigKey("StatusProtocolPort").value << "\n";
		Console::Print << "MaxPlayers: " << getConfigKey("MaxPlayers").value << "\n";
		Console::Print << "OnePlayerOnlinePerAccount: " << getConfigKey("OnePlayerOnlinePerAccount").value << "\n";
		Console::Print << "AllowMultiLogin: " << getConfigKey("AllowMultiLogin").value << "\n";
		Console::Print << "MaxSameIPConnection: " << getConfigKey("MaxSameIPConnection").value << "\n";
		Console::Print << "MaxConnection: " << getConfigKey("MaxConnection").value << "\n";
		Console::Print << "MaxConnectionsPerIP: " << getConfigKey("MaxConnectionsPerIP").value << "\n";
		Console::Print << "MaxConnectionsPerAccount: " << getConfigKey("MaxConnectionsPerAccount").value << "\n";
		Console::Print << "MaxConnectionsPerCharacter: " << getConfigKey("MaxConnectionsPerCharacter").value << "\n";
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
		/// sol documentation says I need to bind this way to avoid having to pass self as first argument.
		///vocation_type.set_function("HpGain", &BK::Vocation::maxHealthPerLevel, BK::Vocation{});

		sol::usertype<Vocation> vocation_type = lua_man.new_usertype<Vocation>("Vocation", sol::call_constructor, sol::factories([]() {return std::make_shared<Vocation>(); }));
		vocation_type["name"], vocation_type["getName"] = &Vocation::getName;
		vocation_type["setName"] = &Vocation::setName;
		vocation_type["id"], vocation_type["getId"] = &Vocation::getId;
		vocation_type["setId"] = &Vocation::setId;
		vocation_type["hpGain"], vocation_type["getHpGain"], vocation_type["healthGain"], vocation_type["getHealthGain"] = &Vocation::getHealthGain;
		vocation_type["setHpGain"], vocation_type["setHealthGain"] = &Vocation::setHealthGain;
		vocation_type["register"] = &Vocation::save;

		sol::usertype<Thing> thing_type = lua_man.new_usertype<Thing>("Thing", sol::call_constructor, sol::factories([]() {return std::make_shared<Thing>(); }));

		sol::usertype<Creature> creature_type = lua_man.new_usertype<Creature>("Creature", sol::call_constructor, sol::factories([]() {return std::make_shared<Creature>(); }));
		creature_type["getCreature"] = &Creature::getCreature;
		creature_type["getPlayer"] = &Creature::getPlayer;
		creature_type[sol::base_classes] = sol::bases<Thing>();

		sol::usertype<Player> player_type = lua_man.new_usertype<Player>("Player", sol::call_constructor, sol::factories([]() {return std::make_shared<Player>(); }));
		player_type["getPlayer"] = &Player::getPlayer;
		player_type["getMonster"] = &Player::getMonster;
		player_type["getHealthPoints"] = &Player::getHealthPoints;
		player_type[sol::base_classes] = sol::bases<Creature>();

	}

} // BKM