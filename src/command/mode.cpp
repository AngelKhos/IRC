#include "server.hpp"
#include <vector>
#include "channel.hpp"
#include <iostream>
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
	if (clients[client_fd]->is_registered == false)
	{
		updateClient(client_fd, Rep.err451(clients[client_fd]->nickName));
		return ;
	}
	if (args.size() <= 0)
	{
		updateClient(client_fd, Rep.err461("MODE", clients[client_fd]->nickName));
		return ;
	}
	std::string target = args[0];
	unsigned long argsIndex = 2;
	std::string validargs;

	Channel *ch = getChannelByName(target, channels);
	if (!ch)
	{
		updateClient(client_fd, Rep.err403(args[0], clients[client_fd]->nickName));
		return ;
	}
	bool plus = false;
	if (args.size() <= 1)
	{
		updateClient(client_fd, Rep.rpl324(*ch, clients[client_fd]->nickName));
		return ;
	}
	if (!ch->isOp(client_fd))
	{
		updateClient(client_fd, Rep.err482(*ch, clients[client_fd]->nickName));
		return ;
	}
	std::string mode = args[1];
	if (mode[0] == '-')
	{
		validargs += "-";
		plus = false;
	}
	else if (mode[0] == '+')
	{
		validargs += "+";
		plus = true;
	}
	else
		return ;

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
			validargs += "i";
		}
		else if (mChar == 'o')
		{
			if (plus)
			{
				if (argsIndex >= args.size())
				{
					updateClient(client_fd, Rep.err461(args[0], clients[client_fd]->nickName));
					continue ;
				}
				ch->opUser(*getUserByNick(args[argsIndex], ch->getUsers()));
				argsIndex++;
			}
			else
			{
				if (argsIndex >= args.size())
				{
					updateClient(client_fd, Rep.err461(args[0], clients[client_fd]->nickName));
					continue ;
				}
				ch->unopUser(*getUserByNick(args[argsIndex], ch->getUsers()));
				argsIndex++;
			}
			validargs += "o";
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
			validargs += "t";
		}
		else if (mChar == 'k')
		{
			if (plus)
			{
				if (argsIndex >= args.size())
				{
					updateClient(client_fd, Rep.err461(args[0], clients[client_fd]->nickName));
					continue ;
				}
				ch->setPw(args[argsIndex]);
				argsIndex++;
			}
			else
				ch->setPw("");
			validargs += "k";
		}
		else if (mChar == 'l')
		{
			if (plus)
			{
				if (argsIndex >= args.size())
				{
					updateClient(client_fd, Rep.err461(args[0], clients[client_fd]->nickName));
					continue ;
				}
				if (atoi(args[argsIndex].c_str()) <= 0)
					return ;
				ch->setLimitUser(atoi(args[argsIndex].c_str()));
				argsIndex++;
			}
			else
			{
				ch->setLimitUser(0);
			}
			validargs += "l";
		}
		else
		{
			updateClient(client_fd, Rep.err472(std::string(&mChar), clients[client_fd]->nickName));
			continue ;
		}
	}
	std::set<Client *> users = ch->getUsers();
	for (std::set<Client *>::iterator u_it = users.begin(); u_it != users.end(); u_it++)
	{
		if (args.size() > 2)
		{
			updateClient((*u_it)->client_fd, clients[client_fd]->prefix() + " MODE " + args[0] + " " + validargs);
			for (std::vector<std::string>::iterator it = args.begin() + 2; it != args.end(); it++)
			{
				updateClient((*u_it)->client_fd, " " + *it);
			}
			updateClient((*u_it)->client_fd, "\r\n");
		}
		else
			updateClient((*u_it)->client_fd, clients[client_fd]->prefix() + " MODE " + args[0] + " " + validargs + "\r\n");
	}
}
