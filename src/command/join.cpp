#include "server.hpp"
#include <vector>
#include <string>
#include "channel.hpp"
#include <set>

std::vector<std::string> lil_split(std::string args)
{
	std::vector<std::string> vec;
	for (size_t i = 0; i <= args.size() + 1; i++)
	{

		if (args[i] != ',' && args[i] != '\0')
			continue ;
		else
		{
			size_t j = 0;
			if (args[0] == ',' || args[0] == '\0')
				j++;
			vec.push_back(args.substr(j, i - j));
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

//TODO handle +l
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
	std::vector<std::string> vec_pw;
	if (args.size() > 1)
		vec_pw= lil_split(args[1]);
	unsigned long pwI = 0;
	for (std::vector<std::string>::iterator it = vec_ch.begin(); it < vec_ch.end(); it++)
	{
		if (Channel::checkName(*it))
		{
			Channel *ch = getChannelByName(*it, channels);
			if (ch != NULL)
			{
				if (ch->isInvOnly())
				{
					if (!ch->isInv(*clients[client_fd]))
					{
						updateClient(client_fd, Rep.err473(*ch, clients[client_fd]->nickName));
						continue ;
					}
					ch->getInvList().erase(clients[client_fd]->nickName);
				}
				if (ch->getPw() != "")
				{
					if (vec_pw.size() > 0)
					{
						if (pwI <= vec_pw.size())
						{
							if (vec_pw[pwI] != ch->getPw())
							{
								updateClient(client_fd, Rep.err475(*ch, clients[client_fd]->nickName));
								continue ;
							}
						}
					}
					else
					{
						updateClient(client_fd, Rep.err475(*ch, clients[client_fd]->nickName));
						continue ;
					}
				}
				if (!checkUserInChannel(clients[client_fd]->userName, ch->getUsers()))
				{
					ch->addUser(*clients[client_fd]);

					std::set<Client *> users = ch->getUsers();
					for (std::set<Client *>::iterator u_it = users.begin(); u_it != users.end(); u_it++)
						updateClient( (*u_it)->client_fd, clients[client_fd]->prefix() + std::string(" JOIN ") 
										+ ch->getName() + std::string("\r\n"));

					updateClient(client_fd, Rep.rpl353(*ch, clients[client_fd]->nickName));
					updateClient(client_fd, Rep.rpl366(*ch, clients[client_fd]->nickName));
					updateClient(client_fd, Rep.rpl324(*ch, clients[client_fd]->nickName));
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

				std::set<Client *> users = ch->getUsers();
					for (std::set<Client *>::iterator u_it = users.begin(); u_it != users.end(); u_it++)
						updateClient( (*u_it)->client_fd, clients[client_fd]->prefix() + std::string(" JOIN ") 
										+ ch->getName() + std::string("\r\n"));

				updateClient(client_fd, Rep.rpl353(*ch, clients[client_fd]->nickName));
				updateClient(client_fd, Rep.rpl366(*ch, clients[client_fd]->nickName));
				updateClient(client_fd, Rep.rpl324(*ch, clients[client_fd]->nickName));
				if (ch->getTopic() == "")
					updateClient(client_fd, Rep.rpl331(*ch, clients[client_fd]->nickName));
				else
					updateClient(client_fd, Rep.rpl332(*ch, clients[client_fd]->nickName));
			}
		}
		else
			updateClient(client_fd, Rep.err403(*it, clients[client_fd]->nickName));
		pwI++;
	}
}