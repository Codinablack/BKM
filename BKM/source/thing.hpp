#ifndef _THING_HPP
#define _THING_HPP

#include <memory>

class Item;
class Creature;

class Thing
{

public:
	Thing() = default;
	virtual ~Thing() = default;

	// non-copyable
	Thing(const Thing&) = delete;
	Thing& operator=(const Thing&) = delete;

	virtual std::shared_ptr<Item> getItem() { return nullptr; }
	virtual std::shared_ptr<Creature> getCreature() { return nullptr; }

};


#endif