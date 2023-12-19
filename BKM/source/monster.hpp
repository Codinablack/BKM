#ifndef _MONSTER_HPP
#define _MONSTER_HPP
#include <iostream>
#include "creature.hpp"

class Monster : public std::enable_shared_from_this<Monster>, public Creature {
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

	auto getMonster() { return std::shared_ptr<Monster>(this); }

private:
};

#endif