#include "numeric_rpl.hpp"
#include <sys/socket.h>
#include <server.hpp>
#include <sstream>

//===============================================================
//reply



const str NumRpl::rpl001(str servName, str nick)
{
	str rpl = SERVER_NAME + str(" 001 ") + nick +
		str(" Welcome to the ") +
		servName +  str(" Internet Relay Chat Network ") +
		str(" ") + nick;
	return rpl + str("\r\n");
}

const str NumRpl::rpl301(str nick, str message)
{
	str rpl = SERVER_NAME + str(" 301 ") + nick +
		str(" :") + message;
	return rpl + str("\r\n");
}

const str NumRpl::rpl324(Channel ch, str nick)
{
	int ulimit = 0;
	str pwd;

	str rpl = SERVER_NAME + str(" 324 ") + nick + str (" ") +
		ch.getName() + str(" +nt");
	if (ch.isInvOnly())
		rpl += str("i");
	if (ch.getUserLimit() != 0)
	{
		ulimit = ch.getUserLimit();
		rpl += str("l");
	}
	if (ch.getPw() != "")
	{
		pwd = ch.getPw();
		rpl += str("k");
	}
	
	if (ulimit != 0)
	{
		rpl += str(" ");
		std::stringstream buff;
		buff << ulimit;
		rpl += buff.str();
	}
	
	if (pwd != "")
		rpl += str(" ") + pwd;
	return rpl + str("\r\n");
}
const str NumRpl::rpl329(Channel ch, long ts, str nick)
{
	std::stringstream buff;
	buff << ts;
	str rpl = SERVER_NAME + str(" 329 ") + nick +
		str(" ") + ch.getName() + str(" ") + buff.str();
	return rpl + str("\r\n");
}

const str NumRpl::rpl331(Channel ch, str nick)
{
	str rpl = SERVER_NAME + str(" 331 ") + nick + str(" ") +
		ch.getName() + str(" :No topic is set");
	return rpl + str("\r\n"); 
}

const str NumRpl::rpl332(Channel ch, str nick)
{
	str rpl = SERVER_NAME + str(" 332 ") + nick + str(" ") +
		ch.getName() + str(" :") + ch.getTopic();
	return rpl + str("\r\n"); 
}

const str NumRpl::rpl333(Channel ch, str author, long ts, str nick)
{
	std::stringstream buff;
	buff << ts;
	str rpl = SERVER_NAME + str(" 333 ") + nick +
		str(" ") + ch.getName() + str(" ") + author + str("") + buff.str();
	return rpl + str("\r\n");
} 

const str NumRpl::rpl341(Channel ch, str invNick, str nick)
{
	str rpl = SERVER_NAME + str(" 332 ") + nick + str(" ") +
		ch.getName() + str(" ") + invNick;
	return rpl + str("\r\n"); 
}

const str NumRpl::rpl353(Channel ch, str nick)
{
	str rpl = SERVER_NAME + str(" 353 ") + nick + str(" ") +
	ch.getName() + str(" :");
	for (std::set<Client *>::iterator it = ch.getUsers().begin(); it != ch.getUsers().end(); it++)
	{
		if (ch.getOpUsers().count(*it) > 0)
			rpl += "@";
		rpl += (*it)->nickName;
		rpl += " ";
	}
	return rpl + str("\r\n"); 
}

const str NumRpl::rpl366(Channel ch, str nick)
{
	str rpl = SERVER_NAME + str(" 366 ") + nick + str(" ") +
	ch.getName() + str(" :End of NAMES list");
	return rpl + str("\r\n");
}

const str NumRpl::rpl372(str nick)
{
	str rpl
	= SERVER_NAME + str(" 372 ") + nick + str(" :- ") + str("      .```.   _.''..\n")
	+ SERVER_NAME + str(" 372 ") + nick + str(" :- ") + str("     ;     ```      ``'.\n")
	+ SERVER_NAME + str(" 372 ") + nick + str(" :- ") + str("     :  d               `.\n")
	+ SERVER_NAME + str(" 372 ") + nick + str(" :- ") + str("     / >,:                \\\n")
	+ SERVER_NAME + str(" 372 ") + nick + str(" :- ") + str("    /.'   `'.,             :\n")
	+ SERVER_NAME + str(" 372 ") + nick + str(" :- ") + str("   /'         ;.   .       ;\n")
	+ SERVER_NAME + str(" 372 ") + nick + str(" :- ") + str("  /          ;  \\ ;     ; /\n")
	+ SERVER_NAME + str(" 372 ") + nick + str(" :- ") + str("             `..;\\:     :'\n")
	+ SERVER_NAME + str(" 372 ") + nick + str(" :- ") + str("            __||   `...,'\n")
	+ SERVER_NAME + str(" 372 ") + nick + str(" :- ") + str("           `-,  )   ||\n")
	+ SERVER_NAME + str(" 372 ") + nick + str(" :- ") + str("            /.^/ ___||\n")
	+ SERVER_NAME + str(" 372 ") + nick + str(" :- ") + str("                '---,_\\\n")
	+ SERVER_NAME + str(" 372 ") + nick + str(" :- ") + str("                   (/ `\\");

	return rpl + str("\r\n");
}
const str NumRpl::rpl375(str nick)
{
	str rpl = SERVER_NAME + str(" 375 ") + nick + str(" :- ") 
	+ SERVER_NAME + str(" Message of the day - ");
	return rpl + str("\r\n");
}
const str NumRpl::rpl376(str nick)
{
	str rpl = SERVER_NAME + str(" 376 ") + nick + str(" :End of MOTD command");
	return rpl + str("\r\n");
}



//===============================================================
//error

const str NumRpl::err401(str nickname, str nick)
{
	str rpl = SERVER_NAME + str(" 401 ") + nick + str(" ") +
		nickname + str(" :No such nick/channel");
	return rpl + str("\r\n"); 
}

const str NumRpl::err403(str chName, str nick)
{
	str rpl = SERVER_NAME + str(" 403 ") + nick + str(" ") +
		chName + str(" :No such channel");
	return rpl + str("\r\n"); 
}

const str NumRpl::err404(str chName, str nick)
{
	str rpl = SERVER_NAME + str(" 404 ") + nick + str(" ") +
		chName + str(" :Cannot send to channel");
	return rpl + str("\r\n"); 
}

const str NumRpl::err407(str target, str nick)
{
	str rpl = SERVER_NAME + str(" 407 ") + nick + str(" ") +
		target + str(" :Duplicate recipients. No message delivered");
	return rpl + str("\r\n"); 
}

const str NumRpl::err411(str command, str nick)
{
	str rpl = SERVER_NAME + str(" 411 ") + nick + 
		str(" :No recipient given (") + command + str(")");
	return rpl + str("\r\n"); 
}

const str NumRpl::err412(str nick)
{
	str rpl = SERVER_NAME + str(" 412 ") + nick + 
		str(" :No text to send");
	return rpl + str("\r\n"); 
}

const str NumRpl::err421(str com, str nick)
{
	str rpl = SERVER_NAME + str(" 421 ") + nick + str(" ") +
		com + str(" :Unknown command");
	return rpl + str("\r\n"); 
}

const str NumRpl::err431(str nick)
{
	str rpl = SERVER_NAME + str(" 431 ") + nick + str(" :No nickname given");
	return rpl + str("\r\n");
}

const str NumRpl::err432(str s, str nick)
{
	str rpl = SERVER_NAME + str(" 432 ") + nick + str(" ") + 
		s + str(" :Erroneus nickname");
	return rpl + str("\r\n");
}

const str NumRpl::err436(str s, str nick)
{
	str rpl = SERVER_NAME + str(" 436 ") + nick + str(" ") + 
		s + str(" :Nickname collision KILL");
	return rpl + str("\r\n");
}

const str NumRpl::err442(str chName, str nick)
{
	str rpl = SERVER_NAME + str(" 442 ") + nick + str(" ") +
		chName + str(" :You're not on that channel");
	return rpl + str("\r\n"); 
}

const str NumRpl::err443(str user, str chName, str nick)
{
	str rpl = SERVER_NAME + str(" 443 ") + nick + str(" ") +
		user + str(" ") +
		chName + str(" :is already on channel");
	return rpl + str("\r\n"); 
}

const str NumRpl::err451(str nick)
{
	str rpl = SERVER_NAME + str(" 451 ") + nick + str(" : You have not registered");
	return rpl + str("\r\n");
}

const str NumRpl::err461(str com, str nick)
{
	str rpl = SERVER_NAME + str(" 461 ") + nick + str(" ") +
		com + str(" :Not enough parameters");
	return rpl + str("\r\n");
}

const str NumRpl::err462(str nick)
{
	str rpl = SERVER_NAME + str(" 462 ") + nick + str(" :You may not reregister");
	return rpl + str("\r\n");
}

const str NumRpl::err471(Channel ch, str nick)
{
	str rpl = SERVER_NAME + str(" 471 ") + nick + str(" ") +
		ch.getName() + str(" :Cannot join channel (+l)");
	return rpl + str("\r\n");
}

const str NumRpl::err472(str c, str nick)
{
	str rpl = SERVER_NAME + str(" 472 ") + nick + str(" ") +
		c + str(" :is unknown mode char to me");
	return rpl + str("\r\n");
}

const str NumRpl::err473(Channel ch, str nick)
{
	str rpl = SERVER_NAME + str(" 473 ") + nick + str(" ") +
		ch.getName() + str(" :Cannot join channel (+i)");
	return rpl + str("\r\n");
}

const str NumRpl::err475(Channel ch, str nick)
{
	str rpl = SERVER_NAME + str(" 475 ") + nick + str(" ") +
		ch.getName() + str(" :Cannot join channel (+k) - bad key");
	return rpl + str("\r\n");
}

const str NumRpl::err482(Channel ch, str nick)
{
	str rpl = SERVER_NAME + str(" 482 ") + nick + str(" ") +
		ch.getName() + str(" :You're not channel operator");
	return rpl + str("\r\n");
}