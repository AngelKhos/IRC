#ifndef NUMERIC_RPL_HPP
#define NUMERIC_RPL_HPP

#include <string>
#include "channel.hpp"
#include "client.hpp"

typedef std::string str;

//TODO
//for join cmd
//ERR_NEEDMOREPARAMS
//ERR_INVITEONLYCHAN
//ERR_NOSUCHCHANNEL

//for user cmd
//ERR_NONICKNAMEGIVEN
//ERR_NICKNAMEINUSE
//ERR_ERRONEUSNICKNAME
//ERR_NICKCOLLISION

//for user cmd
//ERR_NEEDMOREPARAMS
//ERR_ALREADYREGISTRED

class NumRpl
{
	public:
		static void rpl001(str buff, str servName, str nick); //rpl_welcome
		static void rpl353(str buff,Channel ch, str nick); //rpl_namereply
		static void rpl332(str buff, Channel ch, str nick); //rpl_topic

		static void err421(str buff, str com, str nick); //err_unkown_command
};

#endif //NUMERCI_RPL_HPP