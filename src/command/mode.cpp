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
	{
		updateClient(client_fd, Rep.err461(args[0], clients[client_fd]->nickName));
		return ;
	}
	std::string mode = args[1];
	std::string target = args[0];
	unsigned long argsIndex = 2;

	bool plus = false;
	if (mode.size() == 1)
		return ;
	if (mode[0] == '-')
		plus = false;
	else if (mode[0] == '+')
		plus = true;
	else
		return ;
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

	for (size_t i = 1; i < mode.size(); i++)
	{
		char mChar = mode[i];
		if (mChar == 'i')
		{
			if (plus)
			{
				ch->setInvOnly(true);
			}
			else
			{
				ch->setInvOnly(false);
			}
		}
		else if (mChar == 'o')
		{
			if (plus)
			{
				if (argsIndex > args.size())
				{
					updateClient(client_fd, Rep.err461(args[0], clients[client_fd]->nickName));
					return ;
				}
				ch->opUser(*getUserByNick(args[argsIndex], ch->getUsers()));
				argsIndex++;
			}
			else
			{
				if (argsIndex > args.size())
				{
					updateClient(client_fd, Rep.err461(args[0], clients[client_fd]->nickName));
					return ;
				}
				ch->unopUser(*getUserByNick(args[argsIndex], ch->getUsers()));
				argsIndex++;
			}
		}
		else if (mChar == 't')
		{
			if (plus)
			{
				ch->setReTopic(true);
			}
			else
			{
				ch->setReTopic(false);
			}
		}
		else if (mChar == 'k')
		{
			if (plus)
			{
				if (argsIndex > args.size())
				{
					updateClient(client_fd, Rep.err461(args[0], clients[client_fd]->nickName));
					return ;
				}
				ch->setPw(args[argsIndex]);
			}
			else
			{
				ch->setPw("");
			}
		}
		else if (mChar == 'l')
		{
			if (plus)
			{
				if (argsIndex > args.size())
				{
					updateClient(client_fd, Rep.err461(args[0], clients[client_fd]->nickName));
					return ;
				}
				ch->setLimitUser(atoi(args[argsIndex].c_str()));
				argsIndex++;
			}
			else
			{
				ch->setLimitUser(0);
			}
		}
		else
		{
			updateClient(client_fd, Rep.err472(std::string(&mChar), clients[client_fd]->nickName));
			return ;
		}
	}
}
