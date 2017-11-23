//This file was modified by Brian Barion, Justin Mulkin, and Colin Brady with permission from author.

/*
	egpNetPlaygroundGameState.cpp
	By Dan Buckstein

	Test game state implementation.

	Additional contributions: 
*/

#include "egpNet/states/egpNetPlaygroundGameState.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


egpNetPlaygroundGameState::egpNetPlaygroundGameState(int ownerID, int local )
	: egpGameState(ownerID)
{
	memset(m_data, 0, sizeof(m_data));

	// setup all object headers
	unsigned int i;
	for (i = 0; i < objLimit_agent; ++i)
	{
		m_data->m_agentStatus[i].objType = objID_agent;
		m_data->m_agentStatus[i].objID = i;
		m_data->m_agentStatus[i].ownerID = -1;
	}

	// agent at our index is owned by us
	// this may not always be the case, e.g. if one player has multiple agents
	m_data->m_agentStatus[ownerID].ownerID = ownerID;
	m_data->m_agentStatus[ownerID].flags = objFlag_active;

	m_data->m_localGame = local;
}

egpNetPlaygroundGameState::~egpNetPlaygroundGameState()
{

}


int egpNetPlaygroundGameState::SerializeData(char *buffer, const unsigned int bufferCapacity, const unsigned int serializeCount, const double dt) const
{
	char *const start = buffer;
	if (buffer)
	{
		// for each object with the changed flag raised, stuff the data into the buffer
		// advance the write head with each object written
		const NetPlaygroundAgent *agentPtr;
		const NetPlaygroundObjectStatus *agentStatusPtr;
		unsigned int i;
		for (i = 0, agentPtr = m_data->m_agent, agentStatusPtr = m_data->m_agentStatus;
			i < objLimit_agent;
			++i, ++agentPtr, ++agentStatusPtr)
		{
			if (agentStatusPtr->flags & objFlag_change)
			{
				// write object header and advance buffer
				*((NetPlaygroundObjectStatus *)buffer) = *agentStatusPtr;
				buffer += sizeof(NetPlaygroundObjectStatus);
				*((NetPlaygroundAgent *)buffer) = *agentPtr;
				buffer += sizeof(NetPlaygroundAgent);
			}
		}

		// write zero byte to indicate end
		*buffer = 0;
	}
	return (buffer - start);
}

int egpNetPlaygroundGameState::DeserializeData(const char *buffer, const unsigned int bufferCapacity, const unsigned int deserializeCount, const double dt)
{
	const char *const start = buffer;
	if (buffer)
	{
		const NetPlaygroundObjectStatus *objectStatusPtr;

		// unpack objects in buffer, look for newly activated objects
		NetPlaygroundAgent *agentPtr;
		NetPlaygroundObjectStatus *agentStatusPtr;
		while (*buffer)
		{
			// read next object info
			objectStatusPtr = (NetPlaygroundObjectStatus *)buffer;
			buffer += sizeof(NetPlaygroundObjectStatus);
			switch (objectStatusPtr->objType)
			{
				// agent
			case objID_agent:
				agentStatusPtr = m_data->m_agentStatus + objectStatusPtr->objID;
				agentPtr = m_data->m_agent + objectStatusPtr->objID;
				*agentStatusPtr = *objectStatusPtr;
				*agentPtr = *((NetPlaygroundAgent *)buffer);
				buffer += sizeof(NetPlaygroundAgent);

				// Dead Reckoning
				agentPtr->posX += agentPtr->velX * dt;
				agentPtr->posY += agentPtr->velY * dt;

				break;
			}
		}
	}
	return (buffer - start);
}

int egpNetPlaygroundGameState::ProcessInput(const egpKeyboard *keyboard, const egpMouse *mouse, const unsigned int ctrlID, const double dt)
{
	if ((int)ctrlID >= 0)
	{
		NetPlaygroundObjectStatus *status = m_data->m_agentStatus + ctrlID;
		NetPlaygroundAgent *agentPtr = m_data->m_agent + ctrlID, agentPrev[1];
		const float agentMoveSpeed = 100.0f;

		status->ownerID = ctrlID;
		status->flags |= objFlag_active;

		*agentPrev = *agentPtr;

		if (keyboard)
		{
			agentPtr->velX = agentMoveSpeed * ((float)(egpKeyboardKeyIsDown(keyboard, 'd') - egpKeyboardKeyIsDown(keyboard, 'a')));
			agentPtr->velY = agentMoveSpeed * ((float)(egpKeyboardKeyIsDown(keyboard, 'w') - egpKeyboardKeyIsDown(keyboard, 's')));

			// Dead Reckoning
			agentPtr->velX += agentMoveSpeed * ((float)(egpKeyboardKeyIsDown(keyboard, 'd') - egpKeyboardKeyIsDown(keyboard, 'a'))) * dt;
			agentPtr->velY += agentMoveSpeed * ((float)(egpKeyboardKeyIsDown(keyboard, 'w') - egpKeyboardKeyIsDown(keyboard, 's'))) * dt;

			// debug print
			//printf(" vel (%d) = %f, %f \n", ctrlID, agentPtr->velX, agentPtr->velY);

			//printf("PrevAgentVel(%d): %f %f\n", status->objID, agentPrev->velX, agentPrev->velY);
			//printf("AgentVel(%d): %f %f\n", status->objID, agentPtr->velX, agentPtr->velY);
		}
		if (mouse)
		{

		}

		// new scope
		if (m_data->m_localGame)
		{
			NetPlaygroundObjectStatus *status = m_data->m_agentStatus + 1;
			NetPlaygroundAgent *agentPtr = m_data->m_agent + 1, agentPrev[1];
			const float agentMoveSpeed = 100.0f;

			status->ownerID = 1;
			status->flags |= objFlag_active;

			*agentPrev = *agentPtr;

			if (keyboard)
			{
				agentPtr->velX = agentMoveSpeed * ((float)(egpKeyboardKeyIsDown(keyboard, 'l') - egpKeyboardKeyIsDown(keyboard, 'j')));
				agentPtr->velY = agentMoveSpeed * ((float)(egpKeyboardKeyIsDown(keyboard, 'i') - egpKeyboardKeyIsDown(keyboard, 'k')));

				// debug print
				//printf(" vel (%d) = %f, %f \n", ctrlID, agentPtr->velX, agentPtr->velY);

				//printf("PrevAgentVel(%d): %f %f\n", status->objID, agentPrev->velX, agentPrev->velY);
				//printf("AgentVel(%d): %f %f\n", status->objID, agentPtr->velX, agentPtr->velY);
			}
			if (mouse)
			{

			}
		}
	}
	return 0;
}

int egpNetPlaygroundGameState::UpdateState(double dt)
{
	// apply movement to all agents
	NetPlaygroundAgent *agentPtr, agentPrev[1];
	NetPlaygroundObjectStatus *agentStatusPtr;
	unsigned int i;
	for (i = 0, agentPtr = m_data->m_agent, agentStatusPtr = m_data->m_agentStatus; 
		i < objLimit_agent;
		++i, ++agentPtr, ++agentStatusPtr)
	{
		if (agentStatusPtr->flags & objFlag_active)
		{
			// copy to compare new state with previous state
			*agentPrev = *agentPtr;

			// update agent
			agentPtr->posX += (float)dt * agentPtr->velX;
			agentPtr->posY += (float)dt * agentPtr->velY;

			//printf("PrevAgentVel(%d): %f %f\n", agentStatusPtr->objID, agentPrev->velX, agentPrev->velY);
			//printf("AgentVel(%d): %f %f\n", agentStatusPtr->objID, agentPtr->velX, agentPtr->velY);
			//printf("PrevAgentPos(%d): %f %f\n", agentStatusPtr->objID, agentPrev->posX, agentPrev->posY);
			//printf("AgentPos(%d): %f %f\n", agentStatusPtr->objID, agentPtr->posX, agentPtr->posY);

			agentStatusPtr->flags |= objFlag_change;
		}
	}

	return 0;
}
