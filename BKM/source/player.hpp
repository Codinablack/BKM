#ifndef _PLAYER_HPP
#define _PLAYER_HPP
#include <iostream>
#include "creature.hpp"

class Player : public virtual Creature {
public:
	Player() = default;
	~Player() = default;

	// non-copyable
	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;

	virtual std::shared_ptr<Player> getPlayer() final override {
		return std::dynamic_pointer_cast<Player>(shared_from_this());
	}


protected:

private:
};

#endif