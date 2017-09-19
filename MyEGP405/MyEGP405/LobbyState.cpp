#include "LobbyState.h"
#include <stdio.h>

void LobbyState::update()
{
	updateInput();
	updateData();
}

// Render information to screen
void LobbyState::render()
{
}

// Update data based on input and anything else
void LobbyState::updateData()
{
}

void LobbyState::updateNetworking()
{
}

// Process data currently in the input buffer
void LobbyState::processBuffer()
{
}

void LobbyState::ArriveFromPreviousState(StateData * data)
{
	printf("Welcome to the Lobby.\n(C)lient, (S)erver, or (E)xit?\n");
}
