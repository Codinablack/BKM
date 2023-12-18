#define SOL_ALL_SAFETIES_ON 1
#include <lua.hpp>
#include <sol/sol.hpp>
#include "player.hpp"
#include <iostream>
#include "scriptmanager.hpp"


int main() {

	ScriptManager& scriptMan = *ScriptManager::getManager();
	scriptMan.init();
	// we seperated the load from the init for threading later...
	scriptMan.loadScriptDirectory();

	return 0;
}