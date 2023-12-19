#ifndef _PLAYER_HPP
#define _PLAYER_HPP
#include <iostream>
#include "creature.hpp"

class Player : public std::enable_shared_from_this<Player>, public virtual Creature {
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

	auto getPlayer() { return std::shared_ptr<Player>(this); }

protected:

private:

};

#endif