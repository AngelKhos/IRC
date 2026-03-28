#include "server.hpp"
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <signal.h>

void sigHandler(int) {}

int main(int argc, char **argv)
{
	signal(SIGINT, sigHandler);
	if (argc != 3)
		return (1);
	Server ircServer((unsigned short)atoi(argv[1]), argv[2]);

	try
	{
		ircServer.startServer();
	}
	catch(const std::exception& e)
	{
		std::cerr << "Server starting error : " << e.what() << '\n';
	} 
	try
	{
		ircServer.loop();
	}
	catch(const std::exception& e)
	{
		std::cerr << "Server loop error : " << e.what() << '\n';
	}
	
	return (0);
}