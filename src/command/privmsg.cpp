#include "server.hpp"
#include "channel.hpp"

void Server::sendMsg(int src_fd, int dest_fd, std::string message)
{
	std::string msg = clients[src_fd]->prefix() + std::string(" PRIVMSG ") 
				+ clients[dest_fd]->nickName + std::string(" ") + message;
	updateClient(dest_fd, msg + std::string("\r\n"));
}

void Server::sendMsginChan(int src_fd, int dest_fd, std::string chName, std::string message)
{
	std::string msg = clients[src_fd]->prefix() + std::string(" PRIVMSG ") 
				+ chName + std::string(" ") + message;
	updateClient(dest_fd, msg  + std::string("\r\n"));
}

void Server::privmsg(std::vector<std::string> args, int client_fd)
{
	if (args.empty())
		updateClient(client_fd, Rep.err411("PRIVMSG", clients[client_fd]->nickName));
	else if (args.size() == 1 && (getUserByNickInMap(args[0], clients) || getChannelByName(args[0], channels)))
		updateClient(client_fd, Rep.err412(clients[client_fd]->nickName));
	else if (args.size() == 1 && getUserByNickInMap(args[0], clients) == NULL && getChannelByName(args[0], channels) == NULL)
		updateClient(client_fd, Rep.err411("PRIVMSG", clients[client_fd]->nickName));
	else if (args.size() == 2)
	{
		std::vector<std::string> recipient = lil_split(args[0]);

		for (std::vector<std::string>::iterator it = recipient.begin(); it != recipient.end(); it++)
		{
			if (getUserByNickInMap(*it, clients) == NULL && getChannelByName(*it, channels) == NULL)
				updateClient(client_fd, Rep.err401(*it, clients[client_fd]->nickName));
			else if (getUserByNickInMap(*it, clients))
				sendMsg(client_fd ,getUserByNickInMap(*it, clients)->client_fd, args[1]);
			else if (getChannelByName(*it, channels))
			{
				if (getUserByNick(clients[client_fd]->nickName, getChannelByName(*it, channels)->getUsers()) == NULL)
				{
					updateClient(client_fd, Rep.err404(getChannelByName(*it, channels)->getName(), clients[client_fd]->nickName));
				}
				std::set<Client *> Cset = getChannelByName(*it, channels)->getUsers();
				for (std::set<Client *>::iterator iter = Cset.begin(); iter != Cset.end(); iter++)
				{
					if ((*iter)->client_fd != client_fd)
						sendMsginChan(client_fd ,(*iter)->client_fd, getChannelByName(*it, channels)->getName(), args[1]);
				}
			}
		}
	}
	
}