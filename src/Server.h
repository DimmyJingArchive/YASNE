#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <iostream>
#include <string.h>
#include <queue>
#include <thread>
#include <list>

#include "Wrapper.h"

#define MAXPACKETSIZE 4096
#define MAXSOCKETS 64

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
};
}


#endif
