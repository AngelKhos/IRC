#ifndef NUMERCI_RPL_HPP
#define NUMERIC_RPL_HPP

#include <string>

typedef std::string str;

class NumRpl
{
	public:
		static void rpl001(int fd,str servName, str nick);

		static void err421(int fd, str com);
};

#endif //NUMERCI_RPL_HPP