#define SOL_ALL_SAFETIES_ON 1
#include <lua.hpp>
#include <sol/sol.hpp>
#include "player.hpp"
#include <iostream>
#include "scriptmanager.h"


int main() {

	ScriptManager& scriptMan = *ScriptManager::getManager();
	scriptMan.init();

	//sol::state lua;

	//lua.open_libraries(sol::lib::base);

	//// note that you can set a 
	//// userdata before you register a usertype,
	//// and it will still carry 
	//// the right metatable if you register it later

	//// set a variable "p2" of type "player" with 0 ammo
	//lua["p2"] = player(0);

	//// make usertype metatable
	//sol::usertype<player> player_type = lua.new_usertype<player>("player",
	//	// 3 constructors
	//	sol::constructors<player(), player(int), player(int, int)>());

	//// typical member function that returns a variable
	//player_type["shoot"] = &player::shoot;
	//// typical member function
	//player_type["boost"] = &player::boost;

	//// gets or set the value using member variable syntax
	//player_type["hp"] = sol::property(&player::get_hp, &player::set_hp);

	//// read and write variable
	//player_type["speed"] = &player::speed;
	//// can only read from, not write to
	//// .set(foo, bar) is the same as [foo] = bar;
	//player_type.set("bullets", sol::readonly(&player::bullets));

	//lua.script_file("prelude_script.lua");
	//lua.script_file("player_script.lua");
	return 0;
}