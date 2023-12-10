#ifndef _ITEM_HPP
#define _ITEM_HPP
#include "Thing.hpp"
#include <stdint.h>

class Item : virtual public Thing
{
public:
	// Factory member to create item of right type based on type
	static Item* CreateItem(const uint16_t type, uint16_t count = 0);

	// Constructor for items
	Item(const uint16_t type, uint16_t count = 0);
	Item(const Item& i);
	virtual Item* clone() const;

	virtual ~Item() = default;

	// non-assignable
	Item& operator=(const Item&) = delete;

	bool equals(const Item* otherItem) const;

	Item* getItem() override final { return this; }
	const Item* getItem() const override final { return this; }

};

#endif
