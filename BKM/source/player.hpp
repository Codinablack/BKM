#ifndef _PLAYER_HPP
#define _PLAYER_HPP
#include <iostream>
#include "creature.hpp"

class Player : public Creature {
public:

	Player() = default;
	~Player() = default;

	// non-copyable
	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;

	std::shared_ptr<Player> getPlayer() final override { return std::shared_ptr<Player>(this); }

protected:

private:

};

#endif