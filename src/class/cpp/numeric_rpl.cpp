#include "numeric_rpl.hpp"
#include <sys/socket.h>
#include <server.hpp>

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
        rpl += ", ";
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
    = SERVER_NAME + str(" 375 ") + nick + str(" :- ") + str("      .```.   _.''..\n")
    + SERVER_NAME + str(" 375 ") + nick + str(" :- ") + str("     ;     ```      ``'.\n")
    + SERVER_NAME + str(" 375 ") + nick + str(" :- ") + str("     :  d               `.\n")
    + SERVER_NAME + str(" 375 ") + nick + str(" :- ") + str("     / >,:                \\\n")
    + SERVER_NAME + str(" 375 ") + nick + str(" :- ") + str("    /.'   `'.,             :\n")
    + SERVER_NAME + str(" 375 ") + nick + str(" :- ") + str("   /'         ;.   .       ;\n")
    + SERVER_NAME + str(" 375 ") + nick + str(" :- ") + str("  /          ;  \\ ;     ; /\n")
    + SERVER_NAME + str(" 375 ") + nick + str(" :- ") + str("             `..;\\:     :'\n")
    + SERVER_NAME + str(" 375 ") + nick + str(" :- ") + str("            __||   `...,'\n")
    + SERVER_NAME + str(" 375 ") + nick + str(" :- ") + str("           `-,  )   ||\n")
    + SERVER_NAME + str(" 375 ") + nick + str(" :- ") + str("            /.^/ ___||\n")
    + SERVER_NAME + str(" 375 ") + nick + str(" :- ") + str("                '---,_\\\n")
    + SERVER_NAME + str(" 375 ") + nick + str(" :- ") + str("                   (/ `\\");

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

const str NumRpl::err403(str chName, str nick)
{
    str rpl = SERVER_NAME + str(" 403 ") + nick + str(" ") +
        chName + str(" :No such channel");
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

const str NumRpl::err473(Channel ch, str nick)
{
    str rpl = SERVER_NAME + str(" 473 ") + nick + str(" ") +
        ch.getName() + str(" :Cannot join channel (+i)");
    return rpl + str("\r\n");
}

const str NumRpl::err482(Channel ch, str nick)
{
    str rpl = SERVER_NAME + str(" 482 ") + nick + str(" ") +
        ch.getName() + str(" :You're not channel operator");
    return rpl + str("\r\n");
}