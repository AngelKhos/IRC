#include "numeric_rpl.hpp"
#include <sys/socket.h>
#include <server.hpp>

void NumRpl::rpl001(int fd, str servName, str nick)
{
	str rpl = str("001 ") + nick +
		str("Welcome to the ") +
		servName +  str(" Internet Relay Chat Network ") +
		str(" ") + nick;
	send(fd, rpl.c_str(), rpl.size(), 0);
}

void NumRpl::err421(int fd, str com, str nick)
{
	str rpl = str("421 ") + nick + 
		com + str(" :Unknown command");
	send(fd, rpl.c_str(), rpl.size(), 0);
}

void NumRpl::rpl353(int fd,Channel ch, str nick)
{
	str rpl = str("353 ") + nick +
		ch.getName() + str(" :");
	for (std::set<Client *>::iterator it = ch.getUsers().begin(); it != ch.getUsers().end(); it++)
	{
		if (ch.getOpUsers().count(*it) > 0)
			rpl += "@";
		rpl += (*it)->nickName;
	}
	send(fd, rpl.c_str(), rpl.size(), 0);
}

