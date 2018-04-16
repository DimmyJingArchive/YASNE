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
TCP::Client::send(TCP::Data &data)
{
	Expect(TCP::send(mSock, data) != -1, "Send Failed", false);
	return true;
}

TCP::Data
TCP::Client::receive()
{
	TCP::Data reply;
	Expect(TCP::recv(mSock, reply) != -1, "Recieve Failed");
	return reply;
}

void
TCP::Client::close()
{
	TCP::close(mSock);
}
