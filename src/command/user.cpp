#include "server.hpp"

void Server::user(std::vector<std::string> args, int client_fd)
{
	if (args.size() < 3)
	{
		updateClient(client_fd, Rep.err461("USER", clients[client_fd]->nickName));
		return;
	}
	else if (clients[client_fd]->has_user && clients[client_fd]->is_registered)
	{
		updateClient(client_fd, Rep.err462(clients[client_fd]->nickName));
		return;
	}
	else
	{
		clients[client_fd]->has_user = true;
		clients[client_fd]->userName = args[0];
	}
}