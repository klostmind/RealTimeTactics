//============================================================================
// Name        : GameEvents.h
// Author      : AltF4
// Copyright   : 2011, GNU GPLv3
// Description : Game events which have happened on the server, and the
//					client is being notified about.
//============================================================================

#ifndef GAMEEVENTS_H_
#define GAMEEVENTS_H_

#include <stdint.h>
#include "Tile.h"
#include "Unit.h"

namespace RTT
{


//********************************************
//				How to Use This
//********************************************
//	This class defines a set of functions that represent events that have
//		happened on the server, and you (the client) are being notified of.
//		The non-virtual functions in this class are called and executed automatically
//		and take care of all the heavy lifting in terms of game-state and server
//		communication. The virtual functions are called at the end of their
//		respectively paired non-virtual function.
//
//	What you need to do is create override implementations of the virtual functions
//		That is where the UI code goes to "hook into" these events. So, for example,
//		you need to provide an implementation for the function UI_UnitMovedSignal.
//		That is an event telling you that an enemy Unit has moved, and so you need to
//		visually represent for the player

//		Each of the virtual functions begin with "UI_" to help you find them

class GameEvents
{

//Parent function of all Game Events
//	This function listens on the callback socket for Game Events
//	When an event is read, it is processed by calling the sub-functions below
//	returns - true if the game is still running: IE: We should call this function again
//		a return of false indicates that the game is over, and we should enter the MainLobby
bool ProcessGameEvent();


//********************************************
//				Movement Events
//********************************************

//A Unit has moved
//	unitID - The ID of the unit moved
//	destination - The Coordinate of the tile being moved to
//		NOTE: The unit by default should be facing the direction of the last hop moved
//		Calls UnitMovedSignal() which must be defined by the caller
void UnitMoved(Unit *unit, struct Coordinate source,
		struct Coordinate destination);

//Virtual function defined by the caller as a way to hook into this event
virtual void UI_UnitMovedSignal(Unit *unit, struct Coordinate source,
		struct Coordinate destination) = 0;

//A Unit has changed the direction it is facing
//	unitID - The ID of the unit moved
//	direction - The new direction to move the unit to face
//		NOTE: By default, the Unit should be facing the direction moved
//	returns - simple boolean result of whether the move succeeded
void UnitChangedFacing(uint32_t unitID, enum Direction direction);

//Virtual function defined by the caller as a way to hook into this event
virtual void UI_UnitChangedFacingSignal(uint32_t unitID, enum Direction direction) = 0;

//********************************************
//				Action Events
//********************************************

//********************************************
//				Match Events
//********************************************

//The match has ended
void MatchEnded();

virtual void UI_MatchEndedSignal() = 0;

//********************************************
//				Server Events
//********************************************

void MatchPaused();
virtual void UI_MatchPausedSignal() = 0;

void MatchUnPaused();
virtual void UI_MatchUnPaused() = 0;

void OpponentSurrendered();
virtual void UI_OpponentSurrenderedSignal() = 0;

};
}


#endif /* GAMEEVENTS_H_ */
