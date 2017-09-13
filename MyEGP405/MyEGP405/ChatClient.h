#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H

#include <string>

class RakNet::RakPeerInterface;
enum ServerStates;

class ChatClient
{
	private:
		ServerStates mState;
		RakNet::RakPeerInterface *mpPeer;
		std::string mUsername;

		/*
		unsigned int mMaxClients;
		unsigned int mServerPort;
		char mServerIP[32];
		*/

		void RunClientLoop(std::string ip, unsigned int serverPort);
		void RunServerLoop(unsigned int serverPort);

	public:
		void Init();
		void Run();
};
#endif // !CHAT_CLIENT_H
