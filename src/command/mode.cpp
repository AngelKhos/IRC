#include "server.hpp"
#include <vector>
#include "channel.hpp"
#include <cstdlib>

Client *getUserByNick(std::string nick, std::set<Client *>Clients)
{
	for (std::set<Client *>::iterator it = Clients.begin(); it != Clients.end(); it++)
	{
		if (nick == (*it)->nickName)
			return (*it);
	}
	return (NULL);
}

void Server::mode(std::vector<std::string> args, int client_fd)
{
	if (args.size() <= 1)
		//updateClient(client_fd, Rep.err461(args[0], clients[client_fd]->nickName));
		return ;
	std::string mode = args[1];
	std::string target = args[0];
	Channel *ch = getChannelByName(target, channels);
	if (!ch)
	{
		updateClient(client_fd, Rep.err403(args[0], clients[client_fd]->nickName));
		return ;
	}
	if (!ch->isOp(client_fd))
	{
		updateClient(client_fd, Rep.err482(*ch, clients[client_fd]->nickName));
		return ;
	}
	if (mode == "-i")
	{
		if (args.size() > 2)
			return ;
		ch->setInvOnly(false);
	}
	else if (mode == "+i")
	{
		if (args.size() > 2)
			return ;
		ch->setInvOnly(true);
	}
	else if (mode == "-t")
	{}
	else if (mode == "+t")
	{}
	else if (mode == "-k")
	{
		if (args.size() > 2)
			return ;
		else
			ch->setPw("");
	}
	else if (mode == "+k")
	{
		if (args.size() <= 2)
		{
			updateClient(client_fd, Rep.err461(args[0], clients[client_fd]->nickName));
			return ;
		}
		ch->setPw(args[2]);
	}
	else if (mode == "-o")
	{
		if (args.size() <= 2)
		{
			updateClient(client_fd, Rep.err461(args[0], clients[client_fd]->nickName));
			return ;
		}
		Client *c = getUserByNick(args[2], ch->getUsers());
		if (!c)
		{
			updateClient(client_fd, Rep.err403(args[2], clients[client_fd]->nickName));
			return ;
		}
		ch->unopUser(*c);
	}
	else if (mode == "+o")
	{
		if (args.size() <= 2)
		{
			updateClient(client_fd, Rep.err461(args[0], clients[client_fd]->nickName));
			return ;
		}
		Client *c = getUserByNick(args[2], ch->getUsers());
		if (!c)
		{
			updateClient(client_fd, Rep.err403(args[2], clients[client_fd]->nickName));
			return ;
		}
		ch->opUser(*c);
	}
	else if (mode == "-l")
	{
		if (args.size() >= 2)
			return ;
		ch->setLimitUser(0);
	}
	else if (mode == "+l")
	{
		if (args.size() <= 2)
		{
			updateClient(client_fd, Rep.err461(args[0], clients[client_fd]->nickName));
			return ;
		}
		int limit = atoi(args[2].c_str());
		if (limit == 0)
			return ;
		ch->setLimitUser(limit);
	}
	else
		return ;
}