#ifndef NUMERIC_RPL_HPP
#define NUMERIC_RPL_HPP

#include <string>
#include "channel.hpp"
#include "client.hpp"

typedef std::string str;

class NumRpl
{
	public:
		static void rpl001(int fd,str servName, str nick); //rpl_welcome
		static void rpl353(int fd,Channel ch, str nick); //rpl_namereply
		static void rpl332(int fd, Channel ch, str nick); //rpl_topic

		static void err421(int fd, str com, str nick); //err_unkown_command
};

#endif //NUMERCI_RPL_HPP