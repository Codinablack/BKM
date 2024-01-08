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

		ScriptManager scriptMan; // w/ configMan


		void LoadStats() {/* do in cpp */ }
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

	};
}

#endif // _GAME_HPP