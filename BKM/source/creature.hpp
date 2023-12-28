#ifndef _CREATURE_HPP
#define _CREATURE_HPP

#include "thing.hpp"
#include "pointbasedstat.hpp"

namespace BKM {

	class Player;
	class Monster;
	class Npc;

	class Creature : public virtual Thing {
	public:
		Creature() = default;
		virtual ~Creature() = default;

		std::shared_ptr<Creature> getCreature() {
			return std::dynamic_pointer_cast<Creature>(shared_from_this());
		}

		virtual std::shared_ptr<Player> getPlayer() {
			return nullptr;
		}

		virtual std::shared_ptr<Monster> getMonster() {
			return nullptr;
		}

		virtual std::shared_ptr<Npc> getNpc() {
			return nullptr;
		}

		uint64_t const getHealthPoints() { return creature_health.getCurrentPoints(); }
		uint64_t const getMaxHealthPoints() { return creature_health.getMaxPoints(); }
		uint64_t const getManaPoints() { return creature_mana.getCurrentPoints(); }
		uint64_t const getMaxManaPoints() { return creature_mana.getMaxPoints(); }
		uint64_t const getExperiencePoints() { return creature_exp.getCurrentPoints(); }
		uint64_t const getExpToNextLevel() { return creature_exp.getMaxPoints() - creature_exp.getCurrentPoints(); }

	private:
		HealthPoints creature_health;
		ManaPoints creature_mana;
		EnergyPoints creature_energy;
		ExperiencePoints creature_exp;
		SpeedPoints creature_speed;

	};
} // BKM
#endif