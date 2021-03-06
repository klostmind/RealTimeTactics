//============================================================================
// Name        : Unit.h
// Author      : AltF4
// Copyright   : 2011, GNU GPLv3
// Description : Message class sent while player is in the server lobby
//============================================================================

#define SERVER_STATS_SIZE sizeof(uint32_t) + sizeof(uint32_t)

#include "Message.h"
#include "../Match.h"

using namespace std;

namespace RTT
{

enum LobbyResult: char
{
	//Success
	LOBBY_SUCCESS = 0,

	//Failure joining
	LOBBY_MATCH_IS_FULL,
	LOBBY_MATCH_DOESNT_EXIST,
	LOBBY_NOT_ALLOWED_IN,
	LOBBY_ALREADY_IN_MATCH,
};

struct ServerStats
{
	//Total number of matches on server right now
	uint32_t numMatches;
	//Total number of players on server right now
	uint32_t numPlayers;
};

class LobbyMessage: public Message
{
public:
	uint32_t requestedPage;
	uint32_t returnedMatchesCount;
	uint32_t returnedPlayersCount;
	//A list of queried matches
	struct MatchDescription *matchDescriptions;
	//Newly created match
	struct MatchDescription matchDescription;
	struct PlayerDescription *playerDescriptions;
	struct MatchOptions options;
	//ID of a match to join
	uint32_t ID;
	struct ServerStats serverStats;


	LobbyMessage();
	~LobbyMessage();
	LobbyMessage(char *buffer, uint length);
	char *Serialize(uint *length);
};

}
