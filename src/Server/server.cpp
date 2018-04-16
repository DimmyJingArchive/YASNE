#include <iostream>
#include "Server.h"
#include "Wrapper.h"


int main()
{
	TCP::Server tcp;
	TCP::Message msg;
	tcp.setup(60000);
	TCP::Address address;
	while (1)
	{
		msg = tcp.getMessage();
		auto list = tcp.getClient();
		if (msg.message != "")
		{
			fmt::print("Client List:\n");
			for (auto i : list)
			{
				TCP::getpeername(i, address);
				fmt::print("	Client socket fd is {0}, ip is {1}, port is {2}\n", i, address.getAddrStr(), address.getPort());
			}
			fmt::print("Socket: {0}, Message: {1}\n\n", msg.socket, msg.message.c_str());
		}
		usleep(100);
	}

	return 0;
}
