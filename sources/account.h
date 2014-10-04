/////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game  //
/////////////////////////////////////////////////

#ifndef __ACCOUNT__
#define __ACCOUNT__
#include "otsystem.h"
#ifndef __LOGIN_SERVER__

typedef std::list<std::string> Characters;

#else

class GameServer;
struct Character
{
	Character(): server(NULL), status(0) {}
	Character(const std::string& _name, GameServer* _server, int8_t _status):
		name(_name), server(_server), status(_status) {}

	std::string name;
	GameServer* server;
	int8_t status;
};

typedef std::map<std::string, Character> Characters;

#endif

class Account 
{
	public:
		Account() {premiumDays = warnings = number = lastDay = 0;}
		virtual ~Account() {charList.clear();}

		uint16_t premiumDays, warnings;
		uint32_t number, lastDay;
		std::string name, password, recoveryKey, salt;
		Characters charList;
};

#endif
