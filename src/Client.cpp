#include "Client.h"


TCP::Client::Client(std::string address, int port)
{
	mSock = TCP::socket(AF_INET, SOCK_STREAM, 0);
	Expect(mSock != -1, "Socket not connected");
	mServer.setup(address.c_str(), port);
	connect();
}


bool
TCP::Client::connect()
{
	Expect(TCP::connect(mSock, mServer) != -1, "Connection Failed", false);
	return true;
}

bool
TCP::Client::send(std::string data)
{
	Expect(TCP::send(mSock, data.c_str(), strlen(data.c_str()), 0) != -1, "Send Failed", false);
	return true;
}

std::string
TCP::Client::receive()
{
	char buffer[MAXPACKETSIZE];
	std::string reply;
	size_t length;
	Expect(TCP::recv(mSock, buffer, length, 0) != -1, "Recieve Failed", "");
	buffer[length]='\0';
	reply = buffer;
	return reply;
}

void
TCP::Client::close()
{
	TCP::close(mSock);
}
