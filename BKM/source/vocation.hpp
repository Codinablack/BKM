#ifndef _VOCATION_HPP
#define _VOCATION_HPP

#include <map>

namespace BK {

	class Vocation {
	public:

		Vocation() {};

		std::string_view getName() const { return v_name; }
		void setName(const char* name) { v_name = name; }

		uint16_t getId() const {return v_id;}
		void setId(uint16_t id) {v_id = id;}

		uint16_t getHealthGain() const { return v_healthGain;}
		void setHealthGain(uint16_t healthGain) { v_healthGain = healthGain; }


		bool save() {
			if (v_id == NULL) {
				return false;
			}
			return addVocation(v_id, *this);
		}

	private:

		const char* v_name = "Default";
		uint16_t v_id = 0;
		uint16_t v_healthGain = 0;
		uint16_t v_manaGain = 0;
		uint16_t v_speedGain = 0;
		uint16_t v_weightGain = 0;


		inline static std::map<uint16_t, Vocation> vocationList;

		inline static bool addVocation(uint16_t id, Vocation vocation) {
			auto res = vocationList.emplace(id, vocation);
			std::cout << "Adding " << vocation.getName() << " and " << (res.second ? "true" : "false") << std::endl;
			std::cout << "Current list size " << vocationList.size() << std::endl;
			return res.second;
		}

	};
}

#endif
