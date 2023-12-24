#ifndef _NPC_HPP
#define _NPC_HPP
#include <iostream>
#include "creature.hpp"

namespace BKM {

	class Npc : public Creature {
	public:
		Npc() = default;
		~Npc() = default;

		// non-copyable
		Npc(const Npc&) = delete;
		Npc& operator=(const Npc&) = delete;

		virtual std::shared_ptr<Npc> getNpc() final override {
			return std::dynamic_pointer_cast<Npc>(shared_from_this());
		}

	private:

	};
}
#endif