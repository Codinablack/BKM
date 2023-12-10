#ifndef _PLAYER_HPP
#define _PLAYER_HPP
#include <iostream>
#include "creature.hpp"

class Player : public Creature {
public:
	// non-copyable
	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;

	std::unique_ptr<Player> getPlayer() final override { return std::unique_ptr<Player>(this); }

private:
	
};

#endif