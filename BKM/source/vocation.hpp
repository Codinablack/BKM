#ifndef _VOCATION_HPP
#define _VOCATION_HPP

#include <map>

namespace BKM {

	class Vocation {
	public:

		Vocation() {};

		std::string_view getName() const { return v_name; }
		void setName(const char* name) { v_name = name; }

		std::string_view getDescription() const { return v_description; }
		void setDescription(const char* description) { v_description = description; }

		uint16_t getId() const {return v_id;}
		void setId(uint16_t id) {v_id = id;}

		uint16_t getHealthGain() const { return v_healthGain;}
		void setHealthGain(uint16_t healthGain) { v_healthGain = healthGain; }

		uint16_t getManaGain() const { return v_manaGain; }
		void setManaGain(uint16_t manaGain) { v_manaGain = manaGain; }

		uint16_t getSpeedGain() const { return v_speedGain; }
		void setSpeedGain(uint16_t speedGain) { v_speedGain = speedGain; }

		bool save() {
			if (v_id == NULL) {
				return false;
			}
			return addVocation(v_id, *this);
		}

		void addPromotion(uint16_t id) {
			v_promotions.push_back(id);
		}

		std::vector<uint16_t> getPromotions() const {
			return v_promotions;
		}

		void addStatModifier(uint16_t id) {
			v_statModifiers.push_back(id);
		}

		std::vector<uint16_t> getStatModifiers() const {
			return v_statModifiers;
		}

		uint32_t getStatModifier(uint16_t id) const {
			/// todo: return value of modifier(id)
		}

		void addSkillModifier(uint16_t id) {
			v_skillModifiers.push_back(id);
		}

		std::vector<uint16_t> getSkillModifiers() const {
			return v_skillModifiers;
		}

	private:

		const char* v_name = "Default";
		const char* v_description = "";
		uint16_t v_id = 0;
		uint16_t v_healthGain = 0;
		uint16_t v_manaGain = 0;
		uint16_t v_speedGain = 0;
		uint16_t v_weightGain = 0;

		std::vector<uint16_t> v_promotions;
		std::vector<uint16_t> v_statModifiers;
		std::vector<uint16_t> v_skillModifiers;


		inline static std::map<uint16_t, Vocation> vocationList;

		inline static bool addVocation(uint16_t id, Vocation vocation) {
			auto res = vocationList.emplace(id, vocation);
			std::cout << "Adding " << vocation.getName() << " and " << (res.second ? "true" : "false") << std::endl;
			std::cout << "Current list size " << vocationList.size() << std::endl;
			return res.second;
		}
	};
}

#endif //_VOCATION_HPP
