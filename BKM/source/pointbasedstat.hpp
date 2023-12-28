#ifndef _POINTBASEDSTAT_HPP
#define _POINTBASEDSTAT_HPP

#include <cstdint>

namespace BKM {

	class PointBasedStat {
	protected:
		uint64_t max = 0;
		uint64_t current = 0;
		uint64_t bonus = 0;
	public:
		PointBasedStat() : max(0), current(0), bonus(0) {};
		PointBasedStat(uint64_t max) : max(max), current(max), bonus(0) {};
		PointBasedStat(uint64_t max, uint64_t current) : max(max), current(current), bonus(0) {};
		PointBasedStat(uint64_t max, uint64_t current, uint64_t bonus) : max(max), current(current), bonus(bonus) {};

		// For adding points to 'current' field. ex increasing health vs maxhealth.
		virtual void addPoints(uint64_t points) {
			uint64_t limit = getMaxPoints();
			if (current = ~limit) {
				if ((current + points) >= limit) {
					current = limit;
				}
				else {
					current = current + points;
				}
			}
		};

		// for removing points from the 'current' field.
		virtual void removePoints(uint64_t points) {
			if (current > 0 and points <= current) {
				current = current - points;
			}
			else {
				current = 0;
			}
		};

		// for adding to the 'max' field. ex increasing max health vs health.
		virtual void addMaxPoints(uint64_t points) {
			max += points;
		};

		// for removing points from the 'max' field. 
		virtual void removeMaxPoints(uint64_t points) {
			uint64_t diff = max - points;
			if (current > diff) {
				current = diff;
			}
			max -= points;
		};

		// for handling the addition of bonus points, we can add to both 'current' and 'bonus' fields with true for second parameter.
		virtual void addBonus(uint64_t points, bool increaseCurrentPoints = false) {
			if (increaseCurrentPoints) {
				current += points;
			}
			bonus += points;
		};

		// for handling the removal of bonus points.
		virtual void removeBonus(uint64_t points) {
			uint64_t diff = bonus - points;
			if (current > diff) {
				current = diff;
			}
			bonus -= points;
		};


		// Get and Set Methods //

		virtual const uint64_t getCurrentPoints() {
			return current;
		};

		virtual void setCurrentPoints(uint64_t points) {
			current = points;
		};

		virtual const uint64_t getMaxPoints() {
			return bonus + max;
		};

		virtual void setMaxPoints(uint64_t points) {
			max = points;
		};

		virtual const uint64_t getBonusPoints() {
			return bonus;
		};

		virtual void setBonusPoints(uint64_t points) {
			bonus = points;
		};
	};

	class HealthPoints : public PointBasedStat {
	protected:
		uint64_t max = 0;
		uint64_t current = 0;
		uint64_t bonus = 0;

	public:
		HealthPoints() : PointBasedStat() {};
		HealthPoints(uint64_t max) : PointBasedStat(max) {};
		HealthPoints(uint64_t max, uint64_t current) : PointBasedStat(max, current) {};
		HealthPoints(uint64_t max, uint64_t current, uint64_t bonus) : PointBasedStat(max, current, bonus) {};
	};


	class ManaPoints : public PointBasedStat {
	protected:
		uint64_t max = 0;
		uint64_t current = 0;
		uint64_t bonus = 0;

	public:
		ManaPoints() : PointBasedStat() {};
		ManaPoints(uint64_t max) : PointBasedStat(max) {};
		ManaPoints(uint64_t max, uint64_t current) : PointBasedStat(max, current) {};
		ManaPoints(uint64_t max, uint64_t current, uint64_t bonus) : PointBasedStat(max, current, bonus) {};
	};

	class SoulPoints : public PointBasedStat {
	protected:
		uint64_t max = 0;
		uint64_t current = 0;
		uint64_t bonus = 0;

	public:
		SoulPoints() : PointBasedStat() {};
		SoulPoints(uint64_t max) : PointBasedStat(max) {};
		SoulPoints(uint64_t max, uint64_t current) : PointBasedStat(max, current) {};
		SoulPoints(uint64_t max, uint64_t current, uint64_t bonus) : PointBasedStat(max, current, bonus) {};
	};

	class EnergyPoints : public PointBasedStat {
	protected:
		uint64_t max = 0;
		uint64_t current = 0;
		uint64_t bonus = 0;

	public:
		EnergyPoints() : PointBasedStat() {};
		EnergyPoints(uint64_t max) : PointBasedStat(max) {};
		EnergyPoints(uint64_t max, uint64_t current) : PointBasedStat(max, current) {};
		EnergyPoints(uint64_t max, uint64_t current, uint64_t bonus) : PointBasedStat(max, current, bonus) {};
	};

	class SpeedPoints : public PointBasedStat {
	protected:
		uint64_t max = 0;
		uint64_t current = 0;
		uint64_t bonus = 0;

	public:
		SpeedPoints() : PointBasedStat() {};
		SpeedPoints(uint64_t max) : PointBasedStat(max) {};
		SpeedPoints(uint64_t max, uint64_t current) : PointBasedStat(max, current) {};
		SpeedPoints(uint64_t max, uint64_t current, uint64_t bonus) : PointBasedStat(max, current, bonus) {};
	};

	class ExperiencePoints : public PointBasedStat {
	protected:
		uint64_t max = 0;
		uint64_t current = 0;
		uint64_t bonus = 0;

	public:
		ExperiencePoints() : PointBasedStat() {};
		ExperiencePoints(uint64_t max) : PointBasedStat(max) {};
		ExperiencePoints(uint64_t max, uint64_t current) : PointBasedStat(max, current) {};
		ExperiencePoints(uint64_t max, uint64_t current, uint64_t bonus) : PointBasedStat(max, current, bonus) {};
	};

} // BKM

#endif