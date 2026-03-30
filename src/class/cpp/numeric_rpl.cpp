#include "numeric_rpl.hpp"
#include <sys/socket.h>
#include <server.hpp>

//===============================================================
//reply

void NumRpl::rpl001(str buff, str servName, str nick)
{
	str rpl = str("001 ") + nick +
		str("Welcome to the ") +
		servName +  str(" Internet Relay Chat Network ") +
		str(" ") + nick;
	buff += rpl + str("\r\n"); 
}

void NumRpl::rpl353(str buff,Channel ch, str nick)
{
	str rpl = str("353 ") + nick +
	ch.getName() + str(" :");
	for (std::set<Client *>::iterator it = ch.getUsers().begin(); it != ch.getUsers().end(); it++)
	{
		if (ch.getOpUsers().count(*it) > 0)
		rpl += "@";
		rpl += (*it)->nickName;
	}
	buff += rpl + str("\r\n"); 
}

void NumRpl::rpl332(str buff, Channel ch, str nick)
{
	str rpl = str("332 ") + nick + 
	ch.getName() + str(" :") + ch.getTopic();
	buff += rpl + str("\r\n"); 
}

//===============================================================
//error

void NumRpl::err421(str buff, str com, str nick)
{
	str rpl = str("421 ") + nick + 
		com + str(" :Unknown command");
	buff += rpl + str("\r\n"); 
}