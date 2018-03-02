#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <iostream>
#include <string.h>

#include "Wrapper.h"

#define MAXPACKETSIZE 4096

namespace TCP
{
class Client
{
private:
	TCP::Address mServer;
	int mSock;

public:
	Client(std::string address, int port);
	bool connect();
	bool send(std::string data);
	std::string receive();
	void close();
};
}

#endif
