#ifndef NUMERIC_RPL_HPP
#define NUMERIC_RPL_HPP

#include <string>
#include "channel.hpp"
#include "client.hpp"

typedef std::string str;


class NumRpl // les numRpl retournent tous une string qui sera utilisé comme "message" dans updateClient()
{				// dans les methodes des commandes (server::<command>)
	public:
		static const str rpl001(str servName, str nick); //rpl_welcome
		static const str rpl332(Channel ch, str nick); //rpl_topic
		static const str rpl353(Channel ch, str nick); //rpl_namereply

		static const str err403(str chName, str nick); //err_nosuchchannel
		static const str err421(str com, str nick); //err_unknown_command
		static const str err431(str nick); //err_nonicknamegiven
		static const str err432(str s, str nick); //err_erroneusnickname
		static const str err436(str s, str nick); //err_nickcollision
		static const str err451(str nick); //err_notregistered
		static const str err461(str com, str nick); //err_needmoreparams
		static const str err462(str nick); //err_alreadyregistered
		static const str err473(Channel ch, str nick); //err_inviteonlychannel
		
};

#endif //NUMERIC_RPL_HPP