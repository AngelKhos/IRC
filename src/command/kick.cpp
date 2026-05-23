#include "server.hpp"
#include <vector>

void Server::kick(std::vector<std::string> args, int client_fd)
{
	if (clients[client_fd]->is_registered == false)
	{
		updateClient(client_fd, Rep.err451(clients[client_fd]->nickName));
		return ;
	}
	if (args.size() <= 1)
	{
		updateClient(client_fd, Rep.err461("KICK", clients[client_fd]->nickName));
		return ;
	}
	std::vector<std::string> chs = lil_split(args[0]);
	std::vector<std::string> us = lil_split(args[1]);
	std::string com = "";
	if (args.size() == 3)
		com = args[2];
	for (std::vector<std::string>::iterator chIt = chs.begin(); chIt != chs.end(); chIt++)
	{
		Channel *ch = getChannelByName(*chIt, channels);
		if (!ch)
		{
			updateClient(client_fd, Rep.err401(*chIt, clients[client_fd]->nickName));
			continue ;
		}
		if (!getUserByNick(clients[client_fd]->nickName, ch->getUsers()))
		{
			updateClient(client_fd, Rep.err442(*chIt, clients[client_fd]->nickName));
			continue ;
		}
		if (!ch->isOp(client_fd))
		{
			updateClient(client_fd, Rep.err482(*ch, clients[client_fd]->nickName));
			continue ;
		}
		for (std::vector<std::string>::iterator usIt = us.begin(); usIt != us.end(); usIt++)
		{
			Client *u = getUserByNick(*usIt, ch->getUsers());
			if (!ch)
			{
				updateClient(client_fd, Rep.err442(ch->getName(), clients[client_fd]->nickName));
				continue ;
			}
			ch->delUser(*u);
			clients[client_fd]->regChannel.erase(ch->getName());
			if (u->client_fd != client_fd)
				updateClient(u->client_fd, clients[client_fd]->prefix() + " KICK " + ch->getName() + " " + u->nickName + " :" + com + "\r\n");
			std::set<Client *> users = ch->getUsers();
			for (std::set<Client *>::iterator u_it = users.begin(); u_it != users.end(); u_it++)
				updateClient((*u_it)->client_fd, clients[client_fd]->prefix() + " KICK " + ch->getName() + " " + u->nickName + " :" + com + "\r\n");
		}
	}
}