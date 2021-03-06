//============================================================================
// Name        : Unit.h
// Author      : AltF4
// Copyright   : 2011, GNU GPLv3
// Description : Message class which is passed to/from client/server
//============================================================================

#include "AuthMessage.h"
#include "string.h"

using namespace std;
using namespace RTT;

AuthMessage::AuthMessage()
{

}

AuthMessage::AuthMessage(char *buffer, uint32_t length)
{
	if( length < MESSAGE_MIN_SIZE )
	{
		return;
	}

	serializeError = false;

	//Copy the message type
	memcpy(&type, buffer, MESSAGE_MIN_SIZE);
	buffer += MESSAGE_MIN_SIZE;

	switch(type)
	{
		case CLIENT_HELLO:
		{
			//Uses: 1) Message Type
			//		2) Version Number

			uint32_t expectedSize = MESSAGE_MIN_SIZE + (sizeof(uint32_t)*3);
			if( length != expectedSize)
			{
				serializeError = true;
				return;
			}

			memcpy(&softwareVersion.major, buffer, sizeof(softwareVersion.major));
			buffer += sizeof(softwareVersion.major);
			memcpy(&softwareVersion.minor, buffer, sizeof(softwareVersion.minor));
			buffer += sizeof(softwareVersion.minor);
			memcpy(&softwareVersion.rev, buffer, sizeof(softwareVersion.rev));
			buffer += sizeof(softwareVersion.rev);
			break;
		}
		case SERVER_HELLO:
		{
			//Uses: 1) Message type
			//		2) Version Number
			//		3) AuthMechanism

			uint32_t expectedSize = MESSAGE_MIN_SIZE + (sizeof(uint32_t)*3)
					+ sizeof(authMechanism);
			if( length != expectedSize)
			{
				serializeError = true;
				return;
			}

			memcpy(&softwareVersion.major, buffer, sizeof(softwareVersion.major));
			buffer += sizeof(softwareVersion.major);
			memcpy(&softwareVersion.minor, buffer, sizeof(softwareVersion.minor));
			buffer += sizeof(softwareVersion.minor);
			memcpy(&softwareVersion.rev, buffer, sizeof(softwareVersion.rev));
			buffer += sizeof(softwareVersion.rev);

			memcpy(&authMechanism, buffer, sizeof(authMechanism));

			break;
		}
		case CLIENT_AUTH:
		{
			//Uses: 1) Message Type
			//		2) Username
			//		3) Hashed password

			uint32_t expectedSize = MESSAGE_MIN_SIZE + sizeof(username)
					+ sizeof(hashedPassword);
			if( length != expectedSize)
			{
				serializeError = true;
				return;
			}

			memcpy(username, buffer, sizeof(username));
			buffer += sizeof(username);
			memcpy(hashedPassword, buffer, sizeof(hashedPassword));

			break;
		}
		case SERVER_AUTH_REPLY:
		{
			//Uses: 1) authSuccess
			//		2) Your new Player ID
			uint32_t expectedSize = MESSAGE_MIN_SIZE + sizeof(authSuccess) + PLAYER_DESCR_SIZE;
			if( length != expectedSize)
			{
				serializeError = true;
				return;
			}

			memcpy(&authSuccess, buffer, sizeof(authSuccess));
			buffer += sizeof(authSuccess);

			//Player ID that joined
			memcpy(&playerDescription.name, buffer, PLAYER_NAME_SIZE);
			buffer += PLAYER_NAME_SIZE;
			memcpy(&playerDescription.ID, buffer, sizeof(uint32_t));
			buffer += sizeof(uint32_t);
			memcpy(&playerDescription.color, buffer, sizeof(enum TeamColor));
			buffer += sizeof(enum TeamColor);
			memcpy(&playerDescription.team, buffer, sizeof(enum TeamNumber));
			buffer += sizeof(enum TeamNumber);

			break;
		}
		default:
		{
			//error
			serializeError = true;
		}
	}
	serializeError = false;
}

char *AuthMessage::Serialize(uint32_t *length)
{
	char *buffer, *originalBuffer;
	switch(type)
	{
		case CLIENT_HELLO:
		{

			//Uses: 1) Message Type
			//		2) Version Number

			//Allocate the memory and assign it to *buffer
			uint32_t messageSize = MESSAGE_MIN_SIZE	+ (sizeof(uint32_t)*3);
			buffer = (char*)malloc(messageSize);
			originalBuffer = buffer;

			//Put the type in
			memcpy(buffer, &type, MESSAGE_MIN_SIZE);
			buffer += MESSAGE_MIN_SIZE;
			//Version Number
			memcpy(buffer, &softwareVersion.major, sizeof(softwareVersion.major));
			buffer += sizeof(softwareVersion.major);
			memcpy(buffer, &softwareVersion.minor, sizeof(softwareVersion.minor));
			buffer += sizeof(softwareVersion.minor);
			memcpy(buffer, &softwareVersion.rev, sizeof(softwareVersion.rev));
			buffer += sizeof(softwareVersion.rev);

			*length = messageSize;
			return originalBuffer;
		}
		case SERVER_HELLO:
		{
			//Uses: 1) Message Type
			//		2) Version Number
			//		3) AuthMechanism

			uint32_t messageSize = MESSAGE_MIN_SIZE + sizeof(authMechanism)
							+ (sizeof(uint32_t)*3);
			//Allocate the memory and assign it to *buffer
			buffer = (char*)malloc(messageSize);
			originalBuffer = buffer;

			//Put the type in
			memcpy(buffer, &type, MESSAGE_MIN_SIZE);
			buffer += MESSAGE_MIN_SIZE;
			memcpy(buffer, &softwareVersion.major, sizeof(softwareVersion.major));
			buffer += sizeof(softwareVersion.major);
			memcpy(buffer, &softwareVersion.minor, sizeof(softwareVersion.minor));
			buffer += sizeof(softwareVersion.minor);
			memcpy(buffer, &softwareVersion.rev, sizeof(softwareVersion.rev));
			buffer += sizeof(softwareVersion.rev);
			memcpy(buffer, &authMechanism, sizeof(authMechanism));
			*length = messageSize;

			return originalBuffer;
		}
		case CLIENT_AUTH:
		{
			//Uses: 1) Message Type
			//		2) Username
			//		3) Hashed password

			uint32_t messageSize = MESSAGE_MIN_SIZE + sizeof(username)
					+ sizeof(hashedPassword);
			//Allocate the memory and assign it to *buffer
			buffer = (char*)malloc(messageSize);
			originalBuffer = buffer;

			//Put the type in
			memcpy(buffer, &type, MESSAGE_MIN_SIZE);
			buffer += MESSAGE_MIN_SIZE;
			//Username
			memcpy(buffer, username, sizeof(username));
			buffer += sizeof(username);
			//Hashed password
			memcpy(buffer, hashedPassword, sizeof(hashedPassword));

			*length = messageSize;
			return originalBuffer;
		}
		case SERVER_AUTH_REPLY:
		{
			uint32_t messageSize = MESSAGE_MIN_SIZE + sizeof(authSuccess) + PLAYER_DESCR_SIZE;
			//Allocate the memory and assign it to *buffer
			buffer = (char*)malloc(messageSize);
			originalBuffer = buffer;

			//Put the type in
			memcpy(buffer, &type, MESSAGE_MIN_SIZE);
			buffer += MESSAGE_MIN_SIZE;

			//Auth success
			memcpy(buffer, &authSuccess, sizeof(authSuccess));
			buffer += sizeof(authSuccess);

			//Put the player description in
			memcpy(buffer, &playerDescription.name, PLAYER_NAME_SIZE);
			buffer += PLAYER_NAME_SIZE;
			memcpy(buffer, &playerDescription.ID, sizeof(uint32_t));
			buffer += sizeof(uint32_t);
			memcpy(buffer, &playerDescription.color, sizeof(enum TeamColor));
			buffer += sizeof(enum TeamColor);
			memcpy(buffer, &playerDescription.team, sizeof(enum TeamNumber));
			buffer += sizeof(enum TeamNumber);

			*length = messageSize;
			return originalBuffer;
		}
		default:
		{
			//Error
			return NULL;
		}
	}
}
