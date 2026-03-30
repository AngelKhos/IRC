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
	for (Client *c : ch.getUsers())
	{
		if (ch.getOpUsers().count(c) > 0)
			rpl += "@";
		rpl += c->nickName;
	}
	send(fd, rpl.c_str(), rpl.size(), 0);
}

