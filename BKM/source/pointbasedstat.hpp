#ifndef _POINTBASEDSTAT_HPP
#define _POINTBASEDSTAT_HPP

#include <cstdint>

namespace BKM {

	enum class StatModifierType {
		DEFAULT_ADD_CURRENT,
		DEFAULT_SUB_CURRENT,
		DEFAULT_MUL_CURRENT,
		DEFAULT_ADD_MAX,
		DEFAULT_SUB_MAX,
		DEFAULT_MUL_MAX, // should use double, and possibly only allow 1 instance of this type, or better yet, make a user configurable cap? //

	};

	struct StatModifier {
		StatModifier() {}
		uint16_t	id = 0;
		uint16_t	type = 0;
		uint16_t	value = 0;
	};

	class PointBasedStat {
	private:
		const char* name = "";
		uint64_t max = 0;
		uint64_t current = 0;
		std::map<uint16_t, StatModifier> modifiers;
	public:
		PointBasedStat(const char* name, uint64_t max = 0, uint64_t current = 0) : name(name), max(max), current(current) {};

		// For adding points to 'current' field. ex increasing health vs maxhealth.
		void addPoints(uint64_t points) {
			uint64_t limit = getMaxPoints();
			if (current != limit) {
				if ((current + points) >= limit) {
					current = limit;
				}
				else {
					current = current + points;
				}
			}
		};

		// for removing points from the 'current' field.
		void removePoints(uint64_t points) {
			if (current > 0 and points <= current) {
				current = current - points;
			}
			else {
				current = 0;
			}
		};

		// for adding to the 'max' field. ex increasing max health vs health
		// we can add to both 'current' and 'max' fields with true for second parameter.
		void addMaxPoints(uint64_t points, bool increaseCurrentPoints = false) {
			if (increaseCurrentPoints) {
				current += points;
			}
			max += points;
		};

		// for removing points from the 'max' field. 
		void removeMaxPoints(uint64_t points) {
			uint64_t diff = max - points;
			if (current > diff) {
				current = diff;
			}
			max -= points;
		};

		// for adding modifiers.
		void addModifier(StatModifier modifier) {
			modifiers.emplace(modifier.id, modifier);
		}

		// for removing modifiers.
		void removeModifier(StatModifier modifier) {
			modifiers.erase(modifier.id);
		}

		// Get and Set Methods //
		const char* getName() const {
			return name;
		}

		const uint64_t getCurrentPoints() const {
			return current;
		};

		void setCurrentPoints(uint64_t points) {
			current = points;
		};

		const uint64_t getMaxPoints() const {
			return max;
		};

		const std::map<uint16_t, StatModifier>& getModifiers() const {
			return modifiers;
		}

		void setName(const char* newName) {
			name = newName;
		}

		void setMaxPoints(uint64_t points) {
			max = points;
		};
	};

} // BKM

#endif