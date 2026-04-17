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
		static const str rpl301(str nick, str message); //rpl_away
		static const str rpl324(Channel ch, str nick); //rpl_channelmodeis
		static const str rpl329(Channel ch, long ts, str nick); //rpl_creationtime
		static const str rpl331(Channel ch, str nick); //rpl_notopic
		static const str rpl332(Channel ch, str nick); //rpl_topic
		static const str rpl333(Channel ch, str author, long ts, str nick); //rpl_topicwhotime
		static const str rpl341(Channel ch, str invNick, str nick); //rpl_inviting
		static const str rpl353(Channel ch, str nick); //rpl_namereply
		static const str rpl366(Channel ch, str nick); //rpl_endofnames
		static const str rpl372(str nick); //rpl_motd
		static const str rpl375(str nick); //rpl_motdstart
		static const str rpl376(str nick); //rpl_endofmotd

		static const str err401(str nickname, str nick); //err_nosuchnick
		static const str err403(str chName, str nick); //err_nosuchchannel
		static const str err404(str chName, str nick); //err_cannotsendtochan
		static const str err407(str target, str nick); //err_toomanytargets
		static const str err411(str command, str nick); //err_norecipient
		static const str err412(str nick); //err_notexttosend
		static const str err421(str com, str nick); //err_unknown_command
		static const str err431(str nick); //err_nonicknamegiven
		static const str err432(str s, str nick); //err_erroneusnickname
		static const str err436(str s, str nick); //err_nickcollision
		static const str err442(str chName, str nick); //err_notonchannel
		static const str err443(str user, str chName, str nick); //err_useronchannel
		static const str err451(str nick); //err_notregistered
		static const str err461(str com, str nick); //err_needmoreparams
		static const str err462(str nick); //err_alreadyregistered
		static const str err471(Channel ch, str nick); //err_channelisfull
		static const str err472(str c, str nick); //err_unknownmode
		static const str err473(Channel ch, str nick); //err_inviteonlychannel
		static const str err475(Channel ch, str nick); //err_badchannelkey
		static const str err482(Channel ch, str nick); //err_chanoprivsneeded
		
};

#endif //NUMERIC_RPL_HPP