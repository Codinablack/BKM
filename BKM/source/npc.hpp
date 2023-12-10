#ifndef _NPC_HPP
#define _NPC_HPP
#include <iostream>
#include "creature.hpp"

class Npc : public Creature {
public:
	Npc() = default;
	~Npc() = default;
	// non-copyable
	Npc(const Npc&) = delete;
	Npc& operator=(const Npc&) = delete;

	std::shared_ptr<Npc> getNpc() final override { return std::shared_ptr<Npc>(this); }

private:

};

#endif