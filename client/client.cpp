#include <iostream>
#include <fstream>
#include "Client.h"


int main(int argc, char *argv[])
{
	std::string msg = "Hello World";
	TCP::Client tcp("127.0.0.1", 60000);

	while (1)
	{
		std::string rec = tcp.receive();
		if (rec != "")
		{
			std::cout << rec << std::endl;
			break;
		}
		sleep(1);
	}

	while (1)
	{
		std::cout << "Sent" << std::endl;
		tcp.send(msg);
		sleep(1);
	}
	return 0;
}
