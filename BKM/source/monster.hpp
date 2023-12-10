#ifndef _MONSTER_HPP
#define _MONSTER_HPP
#include <iostream>
#include "creature.hpp"

class Monster : public Creature {
public:
	Monster() = default;
	~Monster() = default;
	// non-copyable
	Monster(const Monster&) = delete;
	Monster& operator=(const Monster&) = delete;

	std::shared_ptr<Monster> getMonster() final override { return std::shared_ptr<Monster>(this); }

private:
};

#endif