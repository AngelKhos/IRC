#include "server.hpp"

bool CheckValidName(std::string name)
{
	std::string spchar = "[]{}-_\\|'^";
	for (size_t i = 0; i < name.size(); i++)
	{
		if (isalnum(name[i]) == 0)
			return (false);
		if (spchar.find(name[i]) == std::string::npos)
			return (false);
	}
	return (true);
}

bool CheckClientExist(std::string nick, std::map<int, Client *> clients)
{
	for (std::map<int, Client *>::iterator it = clients.begin();
		it != clients.end();
		it++)
	{
		if ((*it).second->nickName == nick)
			return (true);
	}
	return (false);
}

void Server::nick(std::vector<std::string> args, int client_fd)
{
	if (args.empty())
	{
		updateClient(client_fd, Rep.err431(clients[client_fd]->nickName));
		return ;
	}
	if (!CheckValidName(args[0]))
	{
		updateClient(client_fd, Rep.err432(args[0], clients[client_fd]->nickName));
		return ;
	}
	if (CheckClientExist(args[0], clients))
	{
		updateClient(client_fd, Rep.err436(args[0], clients[client_fd]->nickName));
	}
	else
		clients[client_fd]->nickName = args[0];
	// (void)args;
	// (void)client_fd;
}