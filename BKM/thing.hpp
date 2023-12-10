#ifndef _THING_HPP
#define _THING_HPP

class Thing
{


public:
	constexpr Thing() = default;
	virtual ~Thing() = default;

	// non-copyable
	Thing(const Thing&) = delete;
	Thing& operator=(const Thing&) = delete;

};


#endif