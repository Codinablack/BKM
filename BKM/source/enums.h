#ifndef _ENUMS_H
#define _ENUMS_H

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


#endif // !_ENUMS_H
