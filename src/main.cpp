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

	ircServer.startServer();

	while (ircServer.isRuning())
	{
	}
	return (0);
}