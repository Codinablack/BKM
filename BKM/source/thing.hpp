#ifndef _THING_HPP
#define _THING_HPP

#include <memory>
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

class Item;
class Creature;

class Thing {

public:
	Thing() = default;
	Thing(sol::this_state ts) {
		lua_State* L = ts;
		// references the object that called this function
		// in constructors:
		sol::stack_object selfobj(L, 1);

		// definitely the same
		Thing& self = selfobj.as<Thing>();
		assert(&self == this);
	}
	virtual ~Thing() = default;
};


#endif