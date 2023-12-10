#ifndef _CREATURE_HPP
#define _CREATURE_HPP

#include "thing.hpp"

class Creature : virtual public Thing
{
protected:
	Creature() = default;

public:

	virtual ~Creature() = default;

	// non-copyable
	Creature(const Creature&) = delete;
	Creature& operator=(const Creature&) = delete;

};

#endif