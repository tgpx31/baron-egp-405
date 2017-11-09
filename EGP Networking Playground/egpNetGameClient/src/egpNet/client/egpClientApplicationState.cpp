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
			case ID_TIMESTAMP:
			{
				// get your read time
				RakNet::Time readTime_local = RakNet::GetTime(),
					sentTime_local, sentTime_remote, sentToReadDiff_local;

				// offset to get to user data
				const unsigned char *userData = packet->data + ReadTimeStamp((char*)packet->data, sentTime_local, sentTime_remote);
				const int lastPing = mp_peer->GetLastPing(packet->systemAddress);

				// get local time diff
				sentToReadDiff_local = (readTime_local - sentTime_local);

				// display
				printf("Read Time Local:\t%I64d\tLast Ping:\t%d\n", readTime_local, lastPing);
				printf("Sent Time Local:\t%I64d\tSent Time Remote:\t%I64d\n", sentTime_local, sentTime_remote);
				printf("Sent -> Read Diff:\t%I64d\t\tClock Diff:\t%I64d\n\n", sentToReadDiff_local, (sentTime_local - sentTime_remote));

				switch (userData[0])
				{

					case egpID_currentTime:
					{
						// read time diff from the server and other client
						// display the time diff from the originator
						RakNet::Time sentToRead_remote, sentToRead_other;
						userData += ReadTimeStamp((char*)userData, sentToRead_remote, sentToRead_other);

						int remoteID = *((int*)userData);
						userData += sizeof(int);

						printf("Other (%d) -> Server:\t%I64d\nServer - > Local (%d):\t%I64d\n\n", remoteID, sentToRead_remote, m_myConnectionIndex, sentToReadDiff_local);

						break;
					}

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

			// all messages have a time stamp now
			char msg[64] = { 0 }, *msgPtrTmp = msg + WriteTimeStamp(msg, packetTime_local, packetTime_local),
			*msgPtr = msgPtrTmp + WriteTimeStamp(msgPtrTmp, 0, 0);

			// User data after sync data
			*msgPtrTmp = (char)egpID_currentTime;

			*((int*)msgPtr) = m_myConnectionIndex;
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
