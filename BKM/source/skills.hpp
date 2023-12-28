#ifndef _SKILLS_HPP
#define _SKILLS_HPP

#include "console.hpp"
#include "pointbasedstat.hpp"

namespace BKM {

	class LevelBasedSkill {
		public:

		uint64_t current_level{0};
		PointBasedStat experience{0};

		void addPoints(uint64_t points) {
			if ( (experience.getMaxPoints() - experience.getCurrentPoints()) >= points) {
				experience.addPoints(points);
			} else {
				uint64_t lastLevel = current_level - 1;
				uint64_t nextExp = ((lastLevel+current_level)*30);
				experience.setMaxPoints(nextExp);
				experience.setCurrentPoints((experience.getMaxPoints() - experience.getCurrentPoints()));
			}
		}
	};
}

#endif // _SKILLS_HPP