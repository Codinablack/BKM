#ifndef _AUGMENT_HPP
#define _AUGMENT_HPP

#include <vector>

#include "console.hpp"

namespace BKM {
	/// Forward Declarations
	class PointBasedStat;
	class LevelBasedSkill;



	
	class Augment {
		/// 0 id, 1 ModType, 2 ModValue
		using Modifier = std::array<int64_t, 3>;

		Augment(std::vector<Modifier> mods){
			_modifiers = mods;
		}


		Augment operator + (Augment& ) {/*maybe make it possible to add together augments, but probably better for modifiers*/

		}

	private:
		std::vector<Modifier> _modifiers;
	};

	/// make equippable to items, players, and mounts.

} // namespace BKM

#endif // _AUGMENT_HPP