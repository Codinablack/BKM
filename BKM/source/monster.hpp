#ifndef _MONSTER_HPP
#define _MONSTER_HPP

#include <iostream>

#include "creature.hpp"

namespace BKM {

	class Monster : public virtual Creature {
	public:
		Monster() = default;
		~Monster() = default;

		// non-copyable
		Monster(const Monster&) = delete;
		Monster& operator=(const Monster&) = delete;

		virtual std::shared_ptr<Monster> getMonster() final override {
			return std::dynamic_pointer_cast<Monster>(shared_from_this());
		}

	private:
	};

}

#endif