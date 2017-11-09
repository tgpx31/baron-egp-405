/*
	egpServerApplicationState.cpp
	By Dan Buckstein

	Implementation for server application development.

	Additional contributions:
*/

#include "egpNet/server/egpServerApplicationState.h"

#include <stdio.h>


// internal updates
int egpServerApplicationState::UpdateNetworking()
{
	//if (egpTimerUpdate(m_networkTimer))
	//{
	//	UpdateNetworking();

	//	// ****TO-DO: 
	//	// send time-stamped message
	//	const RakNet::Time sendTime_local = RakNet::GetTime();

	//	// make message containing localized time, to avoid time differences between computers
	//	char msg[32] = { 0 }, *msgPtr = msg + WriteTimeStamp(msg, sendTime_local, sendTime_local);

	//	*(msgPtr++) = 0;// clear end byte

	//	SendPacket(msg, int(msgPtr - msg), -1, 1, 0);
	//	printf("Broadcast message at time %I64d\n", sendTime_local);
	//}

	return egpApplicationState::UpdateNetworking();
}

int egpServerApplicationState::ProcessPacket(const RakNet::Packet *packet)
{
	egpApplicationState::ProcessPacket(packet);

	// additional processing opportunity
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
				// read time diff from the client
				// forward time diff to other clients
				RakNet::Time sentToRead_remote, sentToRead_other;
				userData += ReadTimeStamp((char*)userData, sentToRead_remote, sentToRead_other);

				int remoteID = *((int*)userData);
				userData += sizeof(int);

				printf("Remote (%d) -> Server:\t%I64d\n\n", remoteID, sentToReadDiff_local);

				// make a new message, include the sent to read diff local so that the client has appropriate latency
				// forward
				RakNet::Time packetTime_local = readTime_local;
				char msg[64], *msgPtrTmp = msg + WriteTimeStamp(msg, packetTime_local, packetTime_local),
					*msgPtr = msgPtrTmp + WriteTimeStamp(msgPtrTmp, sentToReadDiff_local, sentToRead_remote);

				*msgPtrTmp = (char)egpID_currentTime;
				*((int*)msgPtr) = remoteID;
				msgPtr += sizeof(int);

				SendPacket(msg, (int)(msgPtr - msg), remoteID, 1, 0);

				printf("Broadcast update message from (%d) at time %I64d\n\n", remoteID, packetTime_local);

				break;
			}

			}

		}	return 1;

		case ID_NEW_INCOMING_CONNECTION: {
			// additional processing for accepted request: 
			// send connection index of new participant
			int i = GetConnectionIndex(packet->systemAddress);
			if (i >= 0)
			{
				char msg[8] = { 0 }, *msgPtr = msg;
				*(msgPtr++) = (char)egpID_connectionIndex;
				*((int *)msgPtr) = i;
				msgPtr += sizeof(i);
				SendPacket(msg, (int)(msgPtr - msg), i, 0, 1);
			}
		}	return 1;


			//	default:
			//		printf("Message with identifier %i has arrived.\n", packet->data[0]);
			//		break;
		}
	}

	return 0;
}



egpServerApplicationState::egpServerApplicationState()
	: egpApplicationState()
{
	// set timers
	egpTimerSet(m_networkTimer, 10.0);

	// start timers
	egpTimerStart(m_networkTimer);
}

egpServerApplicationState::~egpServerApplicationState()
{

}


// callbacks
int egpServerApplicationState::OnIdle()
{
	// ****TO-DO: SERVER IDLE: 
	//	-> process inputs
	//		-> devices
	//		-> remote state(s)
	//		-> local state
	//	-> parent update where appropriate
	if (egpTimerUpdate(m_networkTimer))
	{
		UpdateNetworking();

		// ****TO-DO: 
		// send time-stamped message
		const RakNet::Time sendTime_local = RakNet::GetTime();

		// make message containing localized time, to avoid time differences between computers
		char msg[32] = { 0 }, *msgPtr = msg + WriteTimeStamp(msg, sendTime_local, sendTime_local);

		*(msgPtr++) = 0;// clear end byte

		 //SendPacket(msg, int(msgPtr - msg), -1, 1, 0);

		//printf("Broadcast message at time %I64d\n", sendTime_local);
	}

	// parent update
	egpApplicationState::OnIdle();

	// nothing happened
	return 0;
}
