#include "server.hpp"

void Server::ping(std::vector<std::string> args, int client_fd)
{
	if (args.empty())
		return ;
	std::string rpl = SERVER_NAME + std::string(" PONG kiwi.serv :") + args[0];
	updateClient(client_fd, rpl + std::string("\r\n"));
}

void Server::quit(std::vector<std::string> args, int client_fd)
{
	if (clients[client_fd]->is_registered)
	{
		std::string rpl;
		if (!args.empty())
			rpl = clients[client_fd]->prefix() + std::string(" QUIT ") + args[0] + std::string("\r\n");
		else
			rpl = clients[client_fd]->prefix() + std::string(" QUIT ; User ") 
				+ clients[client_fd]->userName + std::string(" has quit IRC\r\n");

		for (std::set<std::string>::iterator it = clients[client_fd]->regChannel.begin(); it != clients[client_fd]->regChannel.end(); it++)
		{
			Channel *ch = getChannelByName(*it, channels);
			for (std::set<Client *>::iterator iter = ch->getUsers().begin(); iter != ch->getUsers().end(); iter++)
			{
				if ((*iter)->client_fd != client_fd)
					updateClient((*iter)->client_fd, rpl);
			}
		}
		updateClient(client_fd, rpl);
	}
	clients[client_fd]->quit = true;
}