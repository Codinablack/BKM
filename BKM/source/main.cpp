#define SOL_ALL_SAFETIES_ON 1
#include <lua.hpp>
#include <sol/sol.hpp>
#include <iostream>
#include <stdlib.h>

#include "console.hpp"
#include "scriptmanager.hpp"
#include "player.hpp"


int main() {
	using namespace BKM;
	ScriptManager& scriptMan = *ScriptManager::getManager();
	scriptMan.init();
	// we seperated the load from the init for threading later...
	scriptMan.loadScriptDirectory();

	return 0;
}