#ifndef _PLAYER_HPP
#define _PLAYER_HPP
#include <iostream>
#include "creature.hpp"

class Player : public Creature {
public:
	Player() = default;
	Player(sol::this_state ts) {
		lua_State* L = ts;
		// references the object that called this function
		// in constructors:
		sol::stack_object selfobj(L, 1);

		// definitely the same
		Player& self = selfobj.as<Player>();
		assert(&self == this);
	}
	~Player() = default;

	// non-copyable
	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;

	std::shared_ptr<Player> getPlayer() final override { return std::shared_ptr<Player>(this); }

protected:

private:

};

#endif