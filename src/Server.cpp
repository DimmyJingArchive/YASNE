#include "Server.h" 


void
TCP::Server::Task(TCP::Server* server)
{
	fd_set readFD;
	int clientSocket[MAXSOCKETS];
	memset(&clientSocket, 0, sizeof(clientSocket));
	int masterSocket = server->getSock();
	int maxSockFD;
	int newSockFD;
	TCP::Address address;
	char buffer[MAXPACKETSIZE];
	size_t bufferLen;
	while (true)
	{
		FD_ZERO(&readFD);
		FD_SET(masterSocket, &readFD);
		maxSockFD = masterSocket;
		for (int& i : clientSocket)
		{
			if (i > 0)
				FD_SET(i, &readFD);
			if (i > maxSockFD)
				maxSockFD = i;
		}
		Expect(TCP::select(maxSockFD + 1, &readFD, NULL, NULL, NULL) != -1, "Error selecting");
		if (FD_ISSET(masterSocket, &readFD))
		{
			newSockFD = TCP::accept(masterSocket, address);
			Expect(newSockFD != -1, "Address not connected");
			printf("New connection, socket fd is %d, ip is %s, port is %d\n", newSockFD, address.getAddrStr(), address.getPort());
			Expect(TCP::send(newSockFD, "Connection established", 22, 0) != -1, "Error Sending");
			for (int& i : clientSocket)
			{
				if (!i)
				{
					server->addClient(newSockFD);
					i = newSockFD;
					break;
				}
			}
		}
		for (int& i : clientSocket)
		{
			if (FD_ISSET(i, &readFD))
			{
				if (!(TCP::recv(i, buffer, bufferLen, 0)))
				{
					server->removeClient(i);
					TCP::getpeername(i, address);
					printf("Client disconnected, ip %s, port %d\n", address.getAddrStr(), address.getPort());
					TCP::close(i);
					i = 0;
				}
				else
				{
					buffer[bufferLen] = '\0';
					server->addMessage({i, std::string(buffer)});
				}
			}
		}
	}
}

void
TCP::Server::setup(int port)
{
	mSockFD = TCP::socket(AF_INET, SOCK_STREAM, 0);
	Expect(mSockFD != -1, "Socket not connected");
	mServerAddress.setup("127.0.0.1", port);
	Expect(TCP::bind(mSockFD, mServerAddress) != -1, "Bind failed");
	Expect(TCP::listen(mSockFD, 8) != -1, "Listen failed");
	std::thread thread(&Task, this);
	thread.detach();
}

int
TCP::Server::getSock() const
{
	return mSockFD;
}

void
TCP::Server::addMessage(const TCP::Message msg)
{
	mMsgQue.push(msg);
}

TCP::Message
TCP::Server::getMessage()
{
	if (!mMsgQue.empty())
	{
		auto temp = mMsgQue.front();
		mMsgQue.pop();
		return temp;
	}
	else
	{
		return {-1, ""};
	}
}

void
TCP::Server::addClient(int client)
{
	mClientSocket.push_back(client);
}

void
TCP::Server::removeClient(int client)
{
	mClientSocket.remove(client);
}

std::list<int>
TCP::Server::getClient() const
{
	return mClientSocket;
}

void
TCP::Server::close()
{
	TCP::close(mSockFD);
}
