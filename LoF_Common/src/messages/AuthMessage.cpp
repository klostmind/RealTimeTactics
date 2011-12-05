//============================================================================
// Name        : Unit.h
// Author      : AltF4
// Copyright   : 2011, GNU GPLv3
// Description : Message class which is passed to/from client/server
//============================================================================

#include "AuthMessage.h"
#include "string.h"

using namespace std;
using namespace LoF;

AuthMessage::AuthMessage()
{

}

AuthMessage::AuthMessage(char *buffer, uint length)
{
	//Copy the message type
	memcpy(&type, buffer, MESSAGE_MIN_SIZE);
	buffer += MESSAGE_MIN_SIZE;

	switch(type)
	{
		case CLIENT_HELLO:
		{
			//Uses: 1) Message Type
			//		2) Version Number

			memcpy(&softwareVersion, buffer, sizeof(softwareVersion));
			buffer += sizeof(softwareVersion);
			break;
		}
		case SERVER_HELLO:
		{
			//Uses: 1) Version Number
			//		2) AuthMechanism

			memcpy(&softwareVersion, buffer, sizeof(softwareVersion));
			buffer += sizeof(softwareVersion);

			memcpy(&authMechanism, buffer, sizeof(authMechanism));

			break;
		}
		case CLIENT_AUTH:
		{
			//Uses: 1) Message Type
			//		2) Username
			//		3) Hashed password

			memcpy(username, buffer, sizeof(username));
			buffer += sizeof(username);
			memcpy(hashedPassword, buffer, sizeof(hashedPassword));

			break;
		}
		case SERVER_AUTH_REPLY:
		{
			//Uses: 1) authSuccess

			memcpy(&authSuccess, buffer, sizeof(bool));

			break;
		}
		default:
		{
			//error
		}
	}
}

char *AuthMessage::Serialize(uint *length)
{
	char *buffer, *originalBuffer;
	switch(type)
	{
		case CLIENT_HELLO:
		{

			//Uses: 1) Message Type
			//		2) Version Number

			//Allocate the memory and assign it to *buffer
			uint messageSize = MESSAGE_MIN_SIZE	+ sizeof(softwareVersion);
			buffer = (char*)malloc(messageSize);
			originalBuffer = buffer;

			//Put the type in
			memcpy(buffer, &type, MESSAGE_MIN_SIZE);
			buffer += MESSAGE_MIN_SIZE;
			//Version Number
			memcpy(buffer, &softwareVersion,  sizeof(softwareVersion));

			*length = messageSize;
			return originalBuffer;
		}
		case SERVER_HELLO:
		{
			//Uses: 1) Message Type
			//		2) Version Number
			//		3) AuthMechanism

			uint messageSize = MESSAGE_MIN_SIZE + sizeof(authMechanism)
							+ sizeof(softwareVersion);
			//Allocate the memory and assign it to *buffer
			buffer = (char*)malloc(messageSize);
			originalBuffer = buffer;

			//Put the type in
			memcpy(buffer, &type, MESSAGE_MIN_SIZE);
			buffer += MESSAGE_MIN_SIZE;
			memcpy(buffer, &softwareVersion, sizeof(softwareVersion));
			buffer += sizeof(softwareVersion);
			memcpy(buffer, &authMechanism, sizeof(authMechanism));
			*length = messageSize;

			return originalBuffer;
		}
		case CLIENT_AUTH:
		{
			//Uses: 1) Message Type
			//		2) Username
			//		3) Hashed password

			uint messageSize = MESSAGE_MIN_SIZE + sizeof(username)
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
			uint messageSize = MESSAGE_MIN_SIZE + sizeof(bool);
			//Allocate the memory and assign it to *buffer
			buffer = (char*)malloc(messageSize);
			originalBuffer = buffer;

			//Put the type in
			memcpy(buffer, &type, MESSAGE_MIN_SIZE);
			buffer += MESSAGE_MIN_SIZE;

			memcpy(buffer, &authSuccess, sizeof(bool));
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
