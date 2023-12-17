#ifndef _MONSTER_HPP
#define _MONSTER_HPP
#include <iostream>
#include "creature.hpp"

class Monster : public Creature {
public:
	Monster() = default;
	Monster(sol::this_state ts) {
		lua_State* L = ts;
		// references the object that called this function
		// in constructors:
		sol::stack_object selfobj(L, 1);

		// definitely the same
		Monster& self = selfobj.as<Monster>();
		assert(&self == this);
	}
	~Monster() = default;
	// non-copyable
	Monster(const Monster&) = delete;
	Monster& operator=(const Monster&) = delete;

	std::shared_ptr<Monster> getMonster() final override { return std::dynamic_pointer_cast<Monster>(getCreature()); }

private:
};

#endif