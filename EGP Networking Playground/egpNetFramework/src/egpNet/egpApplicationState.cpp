/*
	egpApplicationState.cpp
	By Dan Buckstein

	Base implementation for networked application development.

	Additional contributions: 
*/

#include "egpNet/egpApplicationState.h"

#include <stdio.h>


egpApplicationState::egpApplicationState()
{
	// reset inputs
	egpKeyboardReset(m_keyboard);
	egpMouseReset(m_mouse);

	// reset peer and connections
	mp_peer = 0;
	mp_connection = 0;
	m_maxIncomingConnections = m_numIncomingConnections = m_maxOutgoingConnections = m_numOutgoingConnections = 0;
	m_portIncoming = m_portOutgoing = 0;
	m_myConnectionIndex = -1;

	m_thread->handle = 0;
}

egpApplicationState::~egpApplicationState()
{
	TerminatePeer();
}



// find connection descriptor index
int egpApplicationState::GetConnectionIndex(const RakNet::SystemAddress &address)
{
	if (mp_connection)
	{
		unsigned int i;

		// check against active inbound connections
		for (i = 0; i < m_maxIncomingConnections; ++i)
			if (mp_connection[i].m_active && mp_connection[i].m_address == address)
				return i;

		// check against outbound connection
		if (m_maxOutgoingConnections)
			if (mp_connection[i].m_active && mp_connection[i].m_address == address)
				return i;
	}
	return -1;
}

int egpApplicationState::GetConnectionIndexOpen()
{
	if (mp_connection)
	{
		unsigned int i;

		// find inactive inbound connection
		for (i = 0; i < m_maxIncomingConnections; ++i)
			if (!mp_connection[i].m_active)
				return i;
	}
	return -1;
}


// internal updates
int egpApplicationState::UpdateNetworking()
{
	if (mp_peer)
	{
		unsigned int i;
		RakNet::Packet *packet;
		for (packet = mp_peer->Receive(), i = 0;
			packet;
			ProcessPacket(packet), mp_peer->DeallocatePacket(packet), packet = mp_peer->Receive(), ++i);
		
		// done
		return i;
	}
	return 0;
}

int egpApplicationState::ProcessPacket(const RakNet::Packet *packet)
{
	if (mp_peer && packet)
	{
		// peek at first byte, process common cases
		// return 1 if case was processed
		switch (packet->data[0])
		{

		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			printf("Another client has disconnected.\n");
			return 1;
		case ID_REMOTE_CONNECTION_LOST:
			printf("Another client has lost the connection.\n");
			return 1;
		case ID_REMOTE_NEW_INCOMING_CONNECTION:
			printf("Another client has connected.\n");
			return 1;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			printf("The server is full.\n");
			return 1;
		case ID_CONNECTION_ATTEMPT_FAILED:
			printf("Failed to connect.\n");
			return 1;

		case ID_CONNECTION_REQUEST_ACCEPTED: {
			int i = m_maxIncomingConnections;
			++m_numOutgoingConnections;
			mp_connection[i].m_active = 1;
			mp_connection[i].m_address = packet->systemAddress;
			printf("Our connection request has been accepted.\n");
		}	return 1;
		case ID_NEW_INCOMING_CONNECTION: {
			int i = GetConnectionIndexOpen();
			if (i >= 0)
			{
				++m_numIncomingConnections;
				mp_connection[i].m_active = 1;
				mp_connection[i].m_address = packet->systemAddress;
			}
			printf("A connection is incoming.\n");
		}	return 1;
		case ID_DISCONNECTION_NOTIFICATION: {
			int i = m_maxIncomingConnections;
			if (m_maxOutgoingConnections && packet->systemAddress == mp_connection[i].m_address) {
				--m_numOutgoingConnections;
				mp_connection[i].m_active = 0;
				mp_connection[i].m_address = RakNet::UNASSIGNED_SYSTEM_ADDRESS;
				printf("We have been disconnected.\n");
			}
			else {
				i = GetConnectionIndex(packet->systemAddress);
				if (i >= 0)
				{
					--m_numIncomingConnections;
					mp_connection[i].m_active = 0;
					mp_connection[i].m_address = RakNet::UNASSIGNED_SYSTEM_ADDRESS;
				}
				printf("A client has disconnected.\n");
			}
		}	return 1;
		case ID_CONNECTION_LOST: {
			int i = m_maxIncomingConnections;
			if (m_maxOutgoingConnections && packet->systemAddress == mp_connection[i].m_address) {
				mp_connection[i].m_active = 0;
				mp_connection[i].m_address = RakNet::UNASSIGNED_SYSTEM_ADDRESS;
				printf("Connection lost.\n");
			}
			else {
				i = GetConnectionIndex(packet->systemAddress);
				if (i >= 0)
				{
					mp_connection[i].m_active = 0;
					mp_connection[i].m_address = RakNet::UNASSIGNED_SYSTEM_ADDRESS;
					printf("A client lost the connection.\n");
				}
			}
		}	return 1;

		case egpID_connectionIndex: {
			// receiving connection index
			m_myConnectionIndex = *((int *)(packet->data + 1));
			printf("Received connection index %d.\n", m_myConnectionIndex);
		}	return 1;

		}
	}
	return 0;
}


// send packet
int egpApplicationState::SendPacket(const char *buffer, const unsigned int bufferLength, const int connectionIndex, const int broadcast, const int reliable)
{
	if (mp_peer && mp_connection && buffer && bufferLength)
	{
		// ****TO-DO: make it faster
		const int singleDst = connectionIndex >= 0;
		const bool broadcastFlag = broadcast != 0;
		const PacketPriority priorityFlag = IMMEDIATE_PRIORITY; // HIGH_PRIORITY
		const PacketReliability reliableFlag = reliable ? RELIABLE_SEQUENCED : UNRELIABLE_SEQUENCED;	// RELIABLE_ORDERED
		const RakNet::SystemAddress address = singleDst ? mp_connection[connectionIndex].m_address : RakNet::UNASSIGNED_SYSTEM_ADDRESS;

		// send raw data
		return mp_peer->Send(buffer, bufferLength, priorityFlag, reliableFlag, 0, address, broadcastFlag);
	}
	return 0;
}

int egpApplicationState::SendPacket(const RakNet::BitStream *bs, const int connectionIndex, const int broadcast, const int reliable)
{
	if (mp_peer && mp_connection)
	{
		// ****TO-DO: make it faster
		const int singleDst = connectionIndex >= 0;
		const bool broadcastFlag = broadcast != 0;
		const PacketPriority priorityFlag = IMMEDIATE_PRIORITY; // HIGH_PRIORITY
		const PacketReliability reliableFlag = reliable ? RELIABLE_SEQUENCED : UNRELIABLE_SEQUENCED;	// RELIABLE_ORDERED
		const RakNet::SystemAddress address = singleDst ? mp_connection[connectionIndex].m_address : RakNet::UNASSIGNED_SYSTEM_ADDRESS;

		// send bit stream
		return mp_peer->Send(bs, priorityFlag, reliableFlag, 0, address, broadcastFlag);
	}
	return 0;
}


// write time stamp message ID and precise system time to buffer or bit stream
int egpApplicationState::WriteTimeStamp(char *buffer, const RakNet::Time &t, const RakNet::Time &t0)
{
	const unsigned int ret = (sizeof(char) + sizeof(RakNet::Time) + sizeof(RakNet::Time));
	if (buffer)
	{
		*(buffer++) = (char)(ID_TIMESTAMP);
		RakNet::Time *tPtr = (RakNet::Time *)buffer;
		*(tPtr++) = t;
		*(tPtr++) = t0;
		return ret;
	}
	return 0;
}

int egpApplicationState::WriteTimeStamp(RakNet::BitStream *bs, const RakNet::Time &t, const RakNet::Time &t0)
{
	const unsigned int ret = (sizeof(char) + sizeof(RakNet::Time) + sizeof(RakNet::Time));
	if (bs)
	{
		bs->Write((char)ID_TIMESTAMP);
		bs->Write(t);
		bs->Write(t0);
		return ret;
	}
	return 0;
}

// read time stamp from buffer or bit stream
int egpApplicationState::ReadTimeStamp(const char *buffer, RakNet::Time &t, RakNet::Time &t0)
{
	const unsigned int ret = (sizeof(char) + sizeof(RakNet::Time) + sizeof(RakNet::Time));
	char tag;
	if (buffer)
	{
		tag = *(buffer++);
		const RakNet::Time *tPtr = (RakNet::Time *)buffer;
		t = *(tPtr++);
		t0 = *(tPtr++);
		if (*(buffer + 4) < 0)
			t += 4311744512;	// RakNet seems to be subtracting this number for some stupid reason... and only half the time... what is it doing
		return ret;
	}
	return 0;
}

int egpApplicationState::ReadTimeStamp(RakNet::BitStream *bs, RakNet::Time &t, RakNet::Time &t0)
{
	const unsigned int ret = (sizeof(char) + sizeof(RakNet::Time) + sizeof(RakNet::Time));
	char tag;
	if (bs)
	{
		bs->Read(tag);
		bs->Read(t);
		bs->Read(t0);
		return ret;
	}
	return 0;
}


// common callbacks
int egpApplicationState::OnIdle()
{
	// example idle: update inputs
	egpKeyboardUpdate(m_keyboard);
	egpMouseUpdate(m_mouse);

	// done, indicate that nothing important happened
	return 0;
}

int egpApplicationState::OnKeyPress(unsigned char key)
{
	// press key
	m_keyboard->state.key[key] = 1;

	// done
	return 1;
}

int egpApplicationState::OnKeyRelease(unsigned char key)
{
	// release key
	m_keyboard->state.key[key] = 0;

	// done
	return 0;
}

int egpApplicationState::OnKeyPressSpecial(unsigned char key)
{
	// press key
	m_keyboard->state.special[key] = 1;

	// done
	return 1;
}

int egpApplicationState::OnKeyReleaseSpecial(unsigned char key)
{
	// release key
	m_keyboard->state.special[key] = 0;

	// done
	return 0;
}

int egpApplicationState::OnMouseButtonPress(unsigned char button, int cursorX, int cursorY)
{
	// press button
	m_mouse->state.btn[button] = 1;
	
	// set position
	m_mouse->state.x = cursorX;
	m_mouse->state.y = cursorY;

	// done
	return 1;
}

int egpApplicationState::OnMouseButtonRelease(unsigned char button, int cursorX, int cursorY)
{
	// press button
	m_mouse->state.btn[button] = 0;

	// set position
	m_mouse->state.x = cursorX;
	m_mouse->state.y = cursorY;

	// done
	return 0;
}

int egpApplicationState::OnMouseWheel(int delta, int cursorX, int cursorY)
{
	// change mouse wheel
	m_mouse->state.wheel += delta;

	// set position
	m_mouse->state.x = cursorX;
	m_mouse->state.y = cursorY;

	// done
	return delta;
}

int egpApplicationState::OnMouseMove(int cursorX, int cursorY)
{
	// set position
	m_mouse->state.x = cursorX;
	m_mouse->state.y = cursorY;

	// done
	return 0;
}


// threading
int egpApplicationNetworkingThread(void *param)
{
	egpApplicationState *appState = (egpApplicationState*)param;

	while (appState->mp_peer)
	{
		if (egpTimerUpdate(appState->m_networkTimer))
			appState->UpdateNetworking();
	}

	return 0;
}


// common networking features
int egpApplicationState::InitializePeer(const unsigned int maxIncomingConnections, const int doesConnect, const unsigned short portIncoming)
{
	if (!mp_peer)
	{
		unsigned int totalConnections, connectionSz;

		// determine connections
		m_maxIncomingConnections = maxIncomingConnections;
		m_maxOutgoingConnections = doesConnect ? 1 : 0;
		m_portIncoming = portIncoming;
		totalConnections = m_maxIncomingConnections + m_maxOutgoingConnections;

		// create connection descriptors
		connectionSz = totalConnections * sizeof(ms_ConnectionDescriptor);
		mp_connection = (ms_ConnectionDescriptor *)malloc(connectionSz);
		memset(mp_connection, 0, connectionSz);

		// create and start peer
		mp_peer = RakNet::RakPeerInterface::GetInstance();

		// enable pinging for accurate delays in time stamps
		mp_peer->SetOccasionalPing(true);

		// open socket if we are accepting connections
		RakNet::SocketDescriptor sd = maxIncomingConnections ? RakNet::SocketDescriptor(portIncoming, 0) : RakNet::SocketDescriptor();
		mp_peer->Startup(totalConnections, &sd, 1);
		mp_peer->SetMaximumIncomingConnections(maxIncomingConnections);


		// new thread for networking
		threadLaunch(m_thread, egpApplicationNetworkingThread, this);


		// done
		return 1;
	}
	return 0;
}

int egpApplicationState::TerminatePeer()
{
	if (mp_peer)
	{
		// kill peer
		mp_peer->Shutdown(0);
		RakNet::RakPeerInterface::DestroyInstance(mp_peer);

		// kill connection descriptors
		free(mp_connection);

		// reset variables
		mp_peer = 0;
		mp_connection = 0;
		m_maxIncomingConnections = m_numIncomingConnections = m_maxOutgoingConnections = m_numOutgoingConnections = 0;
		m_portIncoming = m_portOutgoing = 0;
		m_myConnectionIndex = -1;

		// done
		return 1;
	}
	return 0;
}

int egpApplicationState::ConnectPeer(const char address[16], const unsigned short port)
{
	if (mp_peer && address && *address)
	{
		m_portOutgoing = port;
		mp_peer->Connect(address, port, 0, 0);

		// done
		return 1;
	}
	return 0;
}


unsigned short egpApplicationState::GetDefaultPort()
{
	static const unsigned short defaultPort = 1111;

	return defaultPort;
}
