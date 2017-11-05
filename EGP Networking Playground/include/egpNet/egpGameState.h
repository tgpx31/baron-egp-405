/*
	egpGameState.h
	By Dan Buckstein

	Base interface for networked game state. Inherit and build other states 
	to form a finite state machine with transitions.
	NOTE: GAME STATE INTERFACES SHOULD NOT DIFFER WHETHER YOU'RE A SERVER OR 
	CLIENT; IT'S YOUR APPLICATION'S REPRESENTATION OF THE GAME "RIGHT NOW".

	Additional contributions: 
*/

#ifndef __EGP_GAMESTATE_H_
#define __EGP_GAMESTATE_H_


class egpGameState abstract
{
protected: 

	egpGameState();
	virtual ~egpGameState();

};


#endif	// !__EGP_GAMESTATE_H_