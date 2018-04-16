#include <iostream>
#include <fstream>
#include "Client.h"


int main()
{
	std::string str;
	fmt::print("Enter ip address: ");
	std::cin >> str;
	TCP::Client tcp(str.c_str(), 60000);

	while (1)
	{
		TCP::Data rec = tcp.receive();
		if (rec.getDataStr() != "")
		{
			fmt::print("{}\n", rec.getDataStr());
			break;
		}
		sleep(1);
	}

	TCP::Data msg;
	while (1)
	{
		//getline(std::cin, str);
		std::cin >> str;
		msg.setStr(str);
		tcp.send(msg);
		sleep(1);
	}
	return 0;
}
