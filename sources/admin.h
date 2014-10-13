/*
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Credits: TFS Team and TibiaKing.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  */

#ifndef __ADMIN__
#define __ADMIN__
#include "otsystem.h"
#ifdef __OTADMIN__

#include "protocol.h"
#include "textlogger.h"

enum
{
	AP_MSG_LOGIN = 1,
	AP_MSG_ENCRYPTION = 2,
	AP_MSG_KEY_EXCHANGE = 3,
	AP_MSG_COMMAND = 4,
	AP_MSG_PING = 5,
	AP_MSG_KEEP_ALIVE = 6,

	AP_MSG_HELLO = 1,
	AP_MSG_KEY_EXCHANGE_OK = 2,
	AP_MSG_KEY_EXCHANGE_FAILED = 3,
	AP_MSG_LOGIN_OK = 4,
	AP_MSG_LOGIN_FAILED = 5,
	AP_MSG_COMMAND_OK = 6,
	AP_MSG_COMMAND_FAILED = 7,
	AP_MSG_ENCRYPTION_OK = 8,
	AP_MSG_ENCRYPTION_FAILED = 9,
	AP_MSG_PING_OK = 10,
	AP_MSG_MESSAGE = 11,
	AP_MSG_ERROR = 12,
};

enum
{
	CMD_BROADCAST = 1,
	CMD_CLOSE_SERVER = 2,
	CMD_PAY_HOUSES = 3,
	CMD_OPEN_SERVER = 4,
	CMD_SHUTDOWN_SERVER = 5,
	CMD_RELOAD_SCRIPTS = 6,
	//CMD_PLAYER_INFO = 7,
	//CMD_GETONLINE = 8,
	CMD_KICK = 9,
	//CMD_BAN_MANAGER = 10,
	//CMD_SERVER_INFO = 11,
	//CMD_GETHOUSE = 12,
	CMD_SAVE_SERVER = 13,
	CMD_SEND_MAIL = 14,
	CMD_SHALLOW_SAVE_SERVER = 15
};

enum
{
	REQUIRE_LOGIN = 1,
	REQUIRE_ENCRYPTION = 2
};

enum
{
	ENCRYPTION_RSA1024XTEA = 1
};

class NetworkMessage;
class Player;
class RSA;

class Admin
{
	public:
		virtual ~Admin();
		static Admin* getInstance()
		{
			static Admin instance;
			return &instance;
		}

		bool addConnection();
		void removeConnection();

		uint16_t getPolicy() const;
		uint32_t getOptions() const;

		static Item* createMail(const std::string xmlData, std::string& name, uint32_t& depotId);
		bool allow(uint32_t ip) const;

		bool isEncypted() const {return m_encrypted;}
		RSA* getRSAKey(uint8_t type);

	protected:
		Admin();

		int32_t m_currentConnections;
		bool m_encrypted;

		RSA* m_key_RSA1024XTEA;
};

class ProtocolAdmin : public Protocol
{
	public:
#ifdef __ENABLE_SERVER_DIAGNOSTIC__
		static uint32_t protocolAdminCount;
#endif
		virtual void onRecvFirstMessage(NetworkMessage& msg);

		ProtocolAdmin(Connection_ptr connection): Protocol(connection)
		{
			m_state = NO_CONNECTED;
			m_loginTries = m_lastCommand = 0;
			m_startTime = time(NULL);
#ifdef __ENABLE_SERVER_DIAGNOSTIC__
			protocolAdminCount++;
#endif
		}
		virtual ~ProtocolAdmin()
		{
#ifdef __ENABLE_SERVER_DIAGNOSTIC__
			protocolAdminCount--;
#endif
		}

		enum {protocolId = 0xFE};
		enum {isSingleSocket = false};
		enum {hasChecksum = false};
		static const char* protocolName() {return "admin protocol";}

	protected:
		enum ProtocolState_t
		{
			NO_CONNECTED,
			ENCRYPTION_NO_SET,
			ENCRYPTION_OK,
			NO_LOGGED_IN,
			LOGGED_IN
		};

		virtual void parsePacket(NetworkMessage& msg);
		virtual void releaseProtocol();
#ifdef __DEBUG_NET_DETAIL__
		virtual void deleteProtocolTask();
#endif

		// commands
		void adminCommandPayHouses();
		void adminCommandReload(int8_t reload);

		void adminCommandKickPlayer(const std::string& name);
		void adminCommandSendMail(const std::string& xmlData);

	private:
		void addLogLine(LogType_t type, std::string message);

		int32_t m_loginTries;
		ProtocolState_t m_state;
		uint32_t m_lastCommand, m_startTime;
};

#endif

#endif
