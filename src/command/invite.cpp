#include "server.hpp"

Client *getUserByNickInMap(std::string nick, std::map<int, Client *>Clients)
{
	for (std::map<int, Client *>::iterator it = Clients.begin(); it != Clients.end(); it++)
	{
		if (nick == (it->second)->nickName)
			return (it->second);
	}
	return (NULL);
}

void Server::invite(std::vector<std::string> args, int client_fd)
{
	if (args.size() < 2)
	{
		updateClient(client_fd, Rep.err461("INVITE", clients[client_fd]->nickName));
		return ;
	}
	else if (args.size() > 2)
		return ;
	std::string channelName = args[1];
	std::string target = args[0];

	Client *c = getUserByNickInMap(target, clients);
	if (!c)
	{
		updateClient(client_fd, Rep.err401(target, clients[client_fd]->nickName));
		return ;
	}
	Channel *ch = getChannelByName(channelName, channels);
	if (!ch)
	{
		updateClient(client_fd, Rep.err442(channelName, clients[client_fd]->nickName));
		return ;
	}
	if (!ch->isOp(client_fd))
	{
		updateClient(client_fd, Rep.err482(*ch, clients[client_fd]->nickName));
		return ;
	}
	if (getUserByNick(target, ch->getUsers()))
	{
		updateClient(client_fd, Rep.err443(target, channelName, clients[client_fd]->nickName));
		return ;
	}

	updateClient(client_fd, Rep.rpl341(*ch, target, clients[client_fd]->nickName));
	updateClient(c->client_fd, Rep.rpl301(clients[client_fd]->nickName, " is inviting you on " + ch->getName()));
	ch->getInvList().insert(clients[c->client_fd]->nickName);
}