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
	{
		std::cout << "Bag args, try : ./ircserv <port> <pass>" << std::endl;
		return (1);
	}
	Server ircServer((unsigned short)atoi(argv[1]), argv[2]);	

	try
	{
		ircServer.startServer();
	}
	catch(const std::exception& e)
	{
		std::cerr << ANSI_BOLD ANSI_RED "Server starting error" ANSI_RESET << " : " << e.what() << '\n';
	} 
	try
	{
		ircServer.loop();
	}
	catch(const std::exception& e)
	{
		std::cerr << ANSI_BOLD ANSI_RED "Server loop error" << ANSI_RESET << " : " << e.what() << '\n';
	}
	
	return (0);
}