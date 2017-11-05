/*
	egpClientApplicationState.cpp
	By Dan Buckstein

	Implementation for client application development.

	Additional contributions:
*/

#include "egpNet/client/egpClientApplicationState.h"

#include <stdio.h>


// internal updates
int egpClientApplicationState::UpdateNetworking()
{
	return egpApplicationState::UpdateNetworking();
}

int egpClientApplicationState::ProcessPacket(const RakNet::Packet *packet)
{
	egpApplicationState::ProcessPacket(packet);

	// additional processing
	if (mp_peer && packet)
	{
		// peek at first byte, process this peer's cases
		// return 1 if case was processed
		switch (packet->data[0])
		{
			// ****TO-DO: 
			// handle time-stamped messages
		case ID_TIMESTAMP: {
			RakNet::Time readTime_local = RakNet::GetTime(), sentTime_local, sentTime_remote, sentToReadDiff_local;
			
			const unsigned char* userData = packet->data + ReadTimeStamp((char*)packet->data, sentTime_local, sentTime_remote);
			const int lastPing = mp_peer->GetLastPing(packet->systemAddress);

			sentToReadDiff_local = readTime_local - sentTime_local;

			printf("Read time (local) = %I64d (last ping = %d)\n", readTime_local, lastPing);
			printf("Sent time (local) = %I64d, Sent time (remote) = %I64d\n", sentTime_local, sentTime_remote);
			printf("Sent -> Read difff = %I64d, Clock diff = %I64d\n\n", sentToReadDiff_local, sentTime_local - sentTime_remote);

			switch (userData[0])
			{
			case egpID_currentTime:
				// read the time diff from the server AND th other client
				// forward it in a new time-stamped packet to all other clients
				RakNet::Time sentToRead_remote, sentToRead_other;
				userData += ReadTimeStamp((char*)userData, sentToRead_remote, sentToRead_other);
				int otherID = *((int*)userData);
				userData += sizeof(int);

				printf("Other (%d) -> Server = %I64d\n\n", otherID, sentToReadDiff_local);

				// forward
				RakNet::Time packetTime_local = readTime_local;
				char msg[64];
				char* msgPtrTmp = msg + WriteTimeStamp(msg, packetTime_local, packetTime_local);
				char* msgPtr = msgPtrTmp + WriteTimeStamp(msgPtrTmp, sentToReadDiff_local, sentToRead_remote);
				*msgPtrTmp = (char)egpID_currentTime;
				msgPtr += sizeof(int);

				SendPacket(msg, (int)(msgPtr - msg), otherID, true, false);
				printf("Broadcast update message from (%d) at time %I64d\n\n", otherID, packetTime_local);

				break;
			}

		}	return 1;

	//	default:
	//		printf("Message with identifier %i has arrived.\n", packet->data[0]);
	//		break;
		}
	}

	return 0;
}


int egpClientApplicationState::UpdateDisplay()
{
	if (mpk_windowState)
	{
		// EMULATE RENDERING TIME by sleeping for a bit
		// say rendering takes 20 - 24 ms
		const unsigned int sleepTime = 20 + rand() % 5;
		Sleep(sleepTime);

		// done
		return 1;
	}
	return 0;
}



egpClientApplicationState::egpClientApplicationState()
	: egpApplicationState()
	, mpk_windowState(0)
{
	// ****TO-DO: 
	// set timers
	egpTimerSet(m_networkTimer, 10.0);
	egpTimerSet(m_renderTimer, 30.0);

	// start timers
	egpTimerStart(m_networkTimer);
	egpTimerStart(m_renderTimer);
}

egpClientApplicationState::~egpClientApplicationState()
{

}


// callbacks
int egpClientApplicationState::OnIdle()
{
	// ****TO-DO: CLIENT IDLE: 
	//	-> process inputs
	//		-> devices
	//		-> remote state
	//		-> local state
	//	-> rendering (client probably has a render window, server probably does not)
	//	-> parent update where appropriate
	if (egpTimerUpdate(m_networkTimer))
	{
		UpdateNetworking();
	}

	if (egpTimerUpdate(m_renderTimer))
	{
		UpdateDisplay();
	}

	// parent update
	egpApplicationState::OnIdle();

	// nothing happened
	return 0;
}

int egpClientApplicationState::OnKeyPress(unsigned char key)
{
	// process keys
	switch (key)
	{
	case 'C':
		if (InitializePeer(0, 1, GetDefaultPort() + 1))
		{
			char address[16] = { 0 };
			printf("\n Enter the host/server IP address using this 15-character format: ");
			printf("\n  ###.###.###.### \n    -> ");
			fscanf(stdin, "%s", address);
			address[sizeof(address) - 1] = 0;
			ConnectPeer(address, GetDefaultPort());
		}
		break;
	case 'X':
		if (TerminatePeer())
		{
			printf("\n Terminated networking.");
		}
		break;


		// ****TO-DO: 
		// send test time-stamped update message if connected to server
	case 't':
		if (mp_peer && m_myConnectionIndex >= 0)
		{
			// time-stamped message from client: 
			//	-> time stamp message ID
			//	-> time stamp x 2
			//	-> update message ID
			//	-> time diff x 2 (sends as zero because we are the originator)
			//	-> connection index (unique user ID)
			//	-> user data
			const RakNet::Time packetTime_local = RakNet::GetTime();

			char msg[64] = { 0 };
			char *msgPtrTmp = msg + WriteTimeStamp(msg, packetTime_local, packetTime_local);
			char *msgPtr = msgPtrTmp + WriteTimeStamp(msgPtrTmp, 0, 0);
			*msgPtrTmp = (char)egpID_currentTime;
			*(int *)msgPtr = m_myConnectionIndex;
			msgPtr += sizeof(int);

			SendPacket(msg, (int)(msgPtr - msg), 0, 0, 0);

			printf(" Sent update message at time %I64d \n\n", packetTime_local);
		}
		break;
	}

	return 0;
}


// setters
void egpClientApplicationState::SetWindowState(const egpWindowState *windowState)
{
	if (windowState->userData == this)
		mpk_windowState = windowState;
}
