#pragma once

#include <iostream>
#include <string.h>

#include "Wrapper.h"

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
	bool send(TCP::Data &data);
	TCP::Data receive();
	void close();
};
}
