#ifndef _THING_HPP
#define _THING_HPP

class Item;

class Thing
{

public:
	constexpr Thing() = default;
	virtual ~Thing() = default;

	// non-copyable
	Thing(const Thing&) = delete;
	Thing& operator=(const Thing&) = delete;

	virtual Item* getItem() { return nullptr; }
	virtual const Item* getItem() const { return nullptr; }

};


#endif