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

		uint32_t const getHealthPoints() { return c_health.getCurrentPoints(); }
		uint32_t const getMaxHealthPoints() { return c_health.getMaxPoints(); }
		uint32_t const getManaPoints() { return c_mana.getCurrentPoints(); }
		uint32_t const getMaxManaPoints() { return c_mana.getMaxPoints(); }
		uint32_t const getExperiencePoints() { return c_exp.getCurrentPoints(); }
		uint32_t const getExpToNextLevel() { return c_exp.getMaxPoints() - c_exp.getCurrentPoints(); }

	private:
		HealthPoints c_health;
		ManaPoints c_mana;
		ExperiencePoints c_exp;
		SpeedPoints c_speed;

	};
} // BKM
#endif