#ifndef LOBBY_STATE_H
#define LOBBY_STATE_H

#include "State.h"

class LobbyState : public State
{
public:
	void update();
	void render();

private:
	void updateData();
	void updateNetworking();
	void processBuffer();

	void GoToNextState(State* nextState);
	virtual void ArriveFromPreviousState(StateData *data) override;
};

#endif