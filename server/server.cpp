#include <iostream>
#include "Server.h"
#include "Wrapper.h"


int main()
{
	TCP::Server tcp;
	TCP::Message msg;
	tcp.setup(60000);
	sockaddr_in address;
	socklen_t addrlen = sizeof(address);
	while (1)
	{
		msg = tcp.getMessage();
		auto list = tcp.getClient();
		if (msg.message != "")
		{
			std::cout << "Client list: " << std::endl;
			for (auto i : list)
			{
				TCP::getpeername(i, (sockaddr*)&address, &addrlen);
				printf("	Client socket fd is %d, ip is %s, port is %d\n", i, inet_ntoa(address.sin_addr), ntohs(address.sin_port));
			}
			printf("Socket: %d, Message: %s\n", msg.socket, msg.message.c_str());
			std::cout << std::endl;
		}
		usleep(100);
	}

	return 0;
}
