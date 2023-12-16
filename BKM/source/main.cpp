#define SOL_ALL_SAFETIES_ON 1
#include <lua.hpp>
#include <sol/sol.hpp>
#include "player.hpp"
#include <iostream>
#include "scriptmanager.hpp"


int main() {

	ScriptManager& scriptMan = *ScriptManager::getManager();
	scriptMan.init();

	return 0;
}