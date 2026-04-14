#include "server.hpp"
#include <vector>
#include <string>
#include "channel.hpp"
#include <set>

std::vector<std::string> lil_split(std::string args)
{
	std::vector<std::string> vec;
	for (size_t i = 0; i < args.size(); i++)
	{
		if (args[i] != ',' && i != args.size() - 1)
			continue ;
		else
		{
			size_t j = 1;
			size_t j2 = 0;
			if (args[0] == ',')
			{
				j--;
				j2++;
			}
			vec.push_back(args.substr(j2, i + j));
			args.erase(0, i);
			i = 0;
		}
	}
	return (vec);
}

bool isChannelExist(std::string ch, std::set<Channel *>channels)
{
	for (std::set<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		if (ch == (*it)->getName())
			return (true);
	}
	return (false);
}

Channel *getChannelByName(std::string ch, std::set<Channel *>channels)
{
	for (std::set<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		if (ch == (*it)->getName())
			return (*it);
	}
	return (NULL);
}

bool checkUserInChannel(std::string user, std::set<Client *>Clients)
{
	for (std::set<Client *>::iterator it = Clients.begin(); it != Clients.end(); it++)
	{
		if (user == (*it)->userName)
			return (true);
	}
	return (false);
}

void Server::join(std::vector<std::string> args, int client_fd)
{
	if (!clients[client_fd]->is_registered)
	{
		updateClient(client_fd, Rep.err451(clients[client_fd]->nickName));
		return ;
	}
	if (args.empty())
	{
		updateClient(client_fd, Rep.err461("JOIN", clients[client_fd]->nickName));
		return ;
	}
	std::vector<std::string> vec_ch = lil_split(args[0]);
	if (args.size() > 1)
		std::vector<std::string> vec_pw = lil_split(args[1]);

	for (std::vector<std::string>::iterator it = vec_ch.begin(); it < vec_ch.end(); it++)
	{
		if (Channel::checkName(*it))
		{
			Channel *ch = getChannelByName(*it, channels);
			if (ch != NULL)
			{
				if (ch->isInvOnly())
				{
					updateClient(client_fd, Rep.err473(*ch, clients[client_fd]->nickName));
					return ;
				}
				if (!checkUserInChannel(clients[client_fd]->userName, ch->getUsers()))
				{
					ch->addUser(*clients[client_fd]);
					updateClient(client_fd, Rep.rpl353(*ch, clients[client_fd]->nickName));
					updateClient(client_fd, Rep.rpl366(*ch, clients[client_fd]->nickName));
					if (ch->getTopic() == "")
						updateClient(client_fd, Rep.rpl331(*ch, clients[client_fd]->nickName));
					else
						updateClient(client_fd, Rep.rpl332(*ch, clients[client_fd]->nickName));
				}
			}
			else
			{
				ch = new Channel();
				ch->setName(*it);
				channels.insert(ch);
				ch->addUser(*clients[client_fd]);
				ch->opUser(*clients[client_fd]);
				updateClient(client_fd, Rep.rpl353(*ch, clients[client_fd]->nickName));
				updateClient(client_fd, Rep.rpl366(*ch, clients[client_fd]->nickName));
				if (ch->getTopic() == "")
					updateClient(client_fd, Rep.rpl331(*ch, clients[client_fd]->nickName));
				else
					updateClient(client_fd, Rep.rpl332(*ch, clients[client_fd]->nickName));
			}
		}
		else
			updateClient(client_fd, Rep.err403(*it, clients[client_fd]->nickName));
	}
	(void)client_fd;
}