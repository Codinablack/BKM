#ifndef _ENUMS_HPP
#define _ENUMS_HPP
namespace BKM {
	enum ConfigKeyType
	{
		none,
		integer,
		string,
		boolean,
	};


	enum ConfigOption
	{
		ScriptDirectory,
		ServerName,
		ServerPort,
		BindOnlyGlobalAddress,
		LoginProtocolPort,
		GameProtocolPort,
		StatusProtocolPort,
		MaxPlayers,
		OnePlayerOnlinePerAccount,
		AllowMultiLogin,
		MaxSameIPConnection,
		MaxSameIPTime,
		MaxConnection,
		MaxConnectionPerIp,
		MaxConnectionPerAccount,
		MaxConnectionPerCharacter,
	};

	enum class ModifierTarget : uint8_t {
		Default, // default initial type, does nothing.
		Stat,
		Skill
	};

	enum class StatModifierTypes : uint8_t {
		Default, // default initial type, does nothing.
		Points,
		MaxPoints,
		BonusPoints,
	};

	enum class SkillModifierTypes : uint8_t {
		Default, // default initial type, does nothing.
		GainRate,
		Difficulty,

	};


}

#endif // !_ENUMS_H
