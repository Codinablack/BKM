#ifndef _THING_HPP
#define _THING_HPP

#include <memory>
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

class Item;
class Creature;

class Thing : public std::enable_shared_from_this<Thing>
{

public:
	Thing() {};
	virtual ~Thing() = default;

private:


};


#endif