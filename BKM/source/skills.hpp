#ifndef _SKILLS_HPP
#define _SKILLS_HPP

#include "console.hpp"
#include "pointbasedstat.hpp"

namespace BKM {
	/// 0 id, 1 ModType, 2 ModValue
	using SkillModifier = int32_t[3];

	class LevelBasedSkill {
	public:

		void addPoints(uint64_t points) {
			addPoints(points, gainRate);
		}

		void addPoints(uint64_t points, double rate ) {
			points = (uint64_t)((double)points * rate);
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

		void addLevel() {/* do in cpp */ }
		void addLevels() {/* do in cpp */ }
		void addModifier() {/* do in cpp */ }

		void removeLevels() {/* do in cpp */ }
		void removeLevel() {/* do in cpp */ }
		void removeModifier() {/* do in cpp */ }

		uint64_t getLevel() const { return current_level; }
		uint64_t getEffectiveLevel() const { /* do in cpp */ };

		void setLevel(uint64_t level) {/* do in cpp */ };
		void setExperiencePoints(uint64_t points) {/* do in cpp */ };
		void setMaxExperiencePoints(uint64_t points) {/* do in cpp */ };


	private:
		const char* name = "";
		double difficulty = 1.0;  // multiplaction modifier for exp required for level. 1.0 is normal rate
		double gainRate = 1.0;  // multiplication modifier to exp points gained. 1.0 is normal rate

		uint64_t current_level = 0;
		PointBasedStat experience = PointBasedStat(0);

		// can be used to determine exp required for any level
		uint64_t getPointsRequired(uint64_t level) {
			uint64_t lastLevel = level - 1;
			return (uint64_t)((((double)lastLevel + (double)level) * 30.0) * difficulty);
		}

		std::vector<SkillModifier> _modifiers;

	};

}


#endif // _SKILLS_HPP