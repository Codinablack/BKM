#ifndef _SKILLS_HPP
#define _SKILLS_HPP

#include "console.hpp"
#include "pointbasedstat.hpp"

namespace BKM {

	class LevelBasedSkill {
	public:

		double difficulty = 1.0;  // multiplaction modifier for exp required for level. 1.0 is normal rate
		double gainRate = 1.0;  // multiplication modifier to exp points gained. 1.0 is normal rate

		uint64_t current_level = 0;
		PointBasedStat experience = 0;

		void addPoints(uint64_t points) {
			addPoints(points, gainRate);
		}

		void addPoints(uint64_t points, double rate ) {
			points = points * rate;
			if ((experience.getMaxPoints() - experience.getCurrentPoints()) >= points) {
				experience.addPoints(points);
			}
			else {
				uint64_t nextExp = getPointsRequired(current_level + 1);
				experience.setMaxPoints(nextExp);
				experience.setCurrentPoints((experience.getMaxPoints() - experience.getCurrentPoints()));
			}
		}

		void removePoints(uint64_t points) {
			if (experience.getCurrentPoints() >= points) {
				experience.removePoints(points);
			}
			else {
				while (points > 0) {
					if (points < experience.getCurrentPoints()) {
						experience.removePoints(points);
					} 
					else if (points == experience.getCurrentPoints()) {
						uint64_t pointCache = experience.getCurrentPoints();
						experience.removePoints(points);
						points = points - pointCache;
					} 
					else { // if (points > experience.getCurrentPoints())
						uint64_t pointCache = experience.getCurrentPoints();
						experience.removePoints(pointCache);
						points = points - pointCache;
						current_level--;
						experience.setMaxPoints(getPointsRequired(current_level));
					}
				}
			}
		}

		void addLevel() {}
		void removeLevel() {}

		void addLevels() {}
		void removeLevels() {}


	private:
		// can be used to determine exp required for any level
		uint64_t getPointsRequired(uint64_t level) {
			uint64_t lastLevel = level - 1;
			return (((lastLevel + level) * 30) * difficulty);
		}



	};
}

#endif // _SKILLS_HPP