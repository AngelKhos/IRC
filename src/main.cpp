#include "server.hpp"
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

int main(int argc, char **argv)
{
	if (argc != 3)
		return (1);
	Server ircServer((unsigned short)atoi(argv[1]), argv[2]);

	ircServer.setServerFd(socket(AF_INET, SOCK_STREAM, 0));
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(ircServer.getPort());
	address.sin_addr.s_addr = INADDR_ANY;

	bind(ircServer.getServerFd(), (sockaddr*)&address, sizeof(address));

	listen(ircServer.getServerFd(), 10);

	std::cout << "server stating..." << std::endl;

	while (true)
	{
	}
	return (0);
}