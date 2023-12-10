#ifndef _ITEM_HPP
#define _ITEM_HPP
#include "thing.hpp"
#include <cstdint>

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
protected:
	Item() = default;
private:
};

#endif
