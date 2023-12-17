#ifndef _NPC_HPP
#define _NPC_HPP
#include <iostream>
#include "creature.hpp"

class Npc : public Creature {
public:
	Npc() = default;
	Npc(sol::this_state ts) {
		lua_State* L = ts;
		// references the object that called this function
		// in constructors:
		sol::stack_object selfobj(L, 1);

		// definitely the same
		Npc& self = selfobj.as<Npc>();
		assert(&self == this);
	}
	~Npc() = default;
	// non-copyable
	Npc(const Npc&) = delete;
	Npc& operator=(const Npc&) = delete;

	std::shared_ptr<Npc> getNpc() final override { return std::dynamic_pointer_cast<Npc>(getCreature()); }

private:

};

#endif