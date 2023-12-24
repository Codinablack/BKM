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
}

#endif // !_ENUMS_H
