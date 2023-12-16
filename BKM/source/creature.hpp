#ifndef _CREATURE_HPP
#define _CREATURE_HPP

#include "thing.hpp"
#include "pointbasedstat.hpp"

class Player;
class Monster;
class Npc;

class Creature : public virtual Thing
{
public:
	Creature() = default;
	Creature(sol::this_state ts) {
		lua_State* L = ts;
		// references the object that called this function
		// in constructors:
		sol::stack_object selfobj(L, 1);

		// definitely the same
		Creature& self = selfobj.as<Creature>();
		assert(&self == this);
	}
	virtual ~Creature() noexcept = default;

	Creature(const Creature&) = delete;
	Creature& operator=(const Creature&) = delete;

	std::shared_ptr<Creature> getCreature() override final { return std::shared_ptr<Creature>(this); }
	virtual std::shared_ptr<Player> getPlayer() { return nullptr; }
	virtual std::shared_ptr<Npc> getNpc() { return nullptr; }
	virtual std::shared_ptr<Monster> getMonster() { return nullptr; }


	uint32_t const getHealthPoints() { return c_health.getCurrentPoints(); }
	uint32_t const getMaxHealthPoints() { return c_health.getMaxPoints(); }
	uint32_t const getManaPoints() { return c_mana.getCurrentPoints(); }
	uint32_t const getMaxManaPoints() { return c_mana.getMaxPoints(); }
	uint32_t const getExperiencePoints() { return c_exp.getCurrentPoints(); }
	uint32_t const getExpToNextLevel() { return c_exp.getMaxPoints() - c_exp.getCurrentPoints(); } 

protected:



private:
	HealthPoints c_health;
	ManaPoints c_mana;
	ExperiencePoints c_exp;
	SpeedPoints c_speed;
};

#endif