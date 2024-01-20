#ifndef _GAME_HPP
#define _GAME_HPP

#include "console.hpp"

namespace BKM {
	class Game {
	public:

		// AssetManager assetMan; -- w/ modelMan & textureMan;
		// ItemManager itemMan;
		// ShaderManager shaderMan;
		// MapManager mapMan; -- w/ spawnMan & townMan;

		///ScriptManager scriptMan; // w/ configMan


		void LoadStats();
		void LoadSkills() {/* do in cpp */ }
		void LoadVocations() {/* do in cpp */ }
		void LoadModels() {/* do in cpp */ }
		void LoadItems() {/* do in cpp */ }
		void LoadAssets() {/* do in cpp */ }
		void LoadTextures() {/* do in cpp */ }
		void LoadConfig() {/* do in cpp */ }
		void LoadSpells() {/* do in cpp */ }
		void LoadAbilities() {/* do in cpp */ }
		void LoadAugments() {/* do in cpp */ }
		void LoadDatabase() {/* do in cpp */ }


	private:
		std::map<const char*, const char*, std::less<>> config;
		std::map<const char*, const char*, std::less<>> damageTypes;
		std::map<const char*, const char*, std::less<>> skills;
		std::map<const char*, const char*, std::less<>> stats;
		std::map<const char*, const char*, std::less<>> vocations;
		std::map<const char*, const char*, std::less<>> races;
		std::map<const char*, const char*, std::less<>> monsterTypes;
		std::map<const char*, const char*, std::less<>> items;
		std::map<const char*, const char*, std::less<>> spells;
		std::map<const char*, const char*, std::less<>> abilites;
		std::map<const char*, const char*, std::less<>> Augments;
	};
}

#endif // _GAME_HPP