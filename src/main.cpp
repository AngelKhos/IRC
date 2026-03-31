#include "server.hpp"
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <signal.h>

void sigHandler(int) {}

std::vector<std::string> cmd_split(std::string args)
{
	std::vector<std::string> vec;
	for (size_t i = 0; i < args.size(); i++)
	{

		if (args[i] != ':')
		{
			if (args[i] != ' ' || i != args.size() - 1)
				continue ;
			else
			{
				// if (args[i] != '\0')
				i++;
				vec.push_back(args.substr(0, i));
				args.erase(0, i);
				i = 0;
			}
		}
		else
		{
			vec.push_back(args);
			return (vec);
		}
	}
	return (vec);
}

int main(int argc, char **argv)
{
	signal(SIGINT, sigHandler);
	if (argc != 3)
		return (1);
	Server ircServer((unsigned short)atoi(argv[1]), argv[2]);

	std::vector<std::string> vec_test = cmd_split("bonjour ruojnob hello world");
	for (std::vector<std::string>::iterator it = vec_test.begin(); it <= vec_test.end(); it++)
	{
		std::cout << *it << std::endl;
	}

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