#include "server.hpp"
#include <vector>
#include "channel.hpp"
#include <cstdlib>
#include <ctime>

void Server::topic(std::vector<std::string> args, int client_fd)
{
	if (args.size() == 0)
	{
		//updateClient(client_fd, Rep.err461(args[0], clients[client_fd]->nickName));
		return ;
	}
	std::string target = args[0];
	Channel *ch = getChannelByName(target, channels);
	if (!ch)
	{
		updateClient(client_fd, Rep.err403(args[0], clients[client_fd]->nickName));
		return ;
	}
	if (getUserByNick(clients[client_fd]->nickName, ch->getUsers()) == NULL)
	{
		//RPL A FAIRE//////////////////////////////////////////
		//updateClient(client_fd, Rep.err42(*ch, clients[client_fd]->nickName));
		return ;
	}
	if (args.size() == 1)
	{
		if (ch->getTopic() == "")
		{
			updateClient(client_fd, Rep.rpl331(*ch, clients[client_fd]->nickName));
			return ;
		}
		else
		{
			updateClient(client_fd, Rep.rpl332(*ch, clients[client_fd]->nickName));
			return ;
		}
	}
	else if (args.size() > 1)
	{
		if (ch->getReTopic())
		{
			if (!ch->isOp(client_fd))
			{
				updateClient(client_fd, Rep.err482(*ch, clients[client_fd]->nickName));
				return ;
			}
		}
		ch->setTopic(args[1]);
		ch->setTopicAutor(clients[client_fd]->nickName);
		ch->setTopicUpdate(std::time(NULL));
	}
}