#pragma once

#include <iostream>
#include <string.h>
#include <queue>
#include <thread>
#include <list>

#include "Wrapper.h"

namespace TCP
{
struct Message
{
	int socket;
	std::string message;
};

class Server
{
private:
	int mSockFD;
	TCP::Address mServerAddress;
	std::queue<TCP::Message> mMsgQue;
	std::list<int> mClientSocket;

public:
	void setup(int port);
	int getSock() const;
	void addMessage(const TCP::Message msg);
	TCP::Message getMessage();
	void addClient(int socket);
	void removeClient(int socket);
	std::list<int> getClient() const;
	void close();

private:
	static void Task(TCP::Server* server);
	static void acceptSocket(int masterSocket, TCP::Address &address, int (&clientSocket)[MAXSOCKETS], TCP::Server* server);
	static void acceptMessage(int (&clientSocket)[MAXSOCKETS], TCP::Address &address, TCP::Server *server, fd_set &readFD);
};
}
