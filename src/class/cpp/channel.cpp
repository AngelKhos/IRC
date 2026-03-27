#include "channel.hpp"
#include "client.hpp"

Channel::Channel() : name("") {}

Channel::Channel(std::string Pname) : name(Pname) {}

Channel::~Channel() {}

//check if the name have max of 200 char, # or & at the begin and no space or ','
bool Channel::checkName(std::string Pname)
{
	if (Pname.length() > CH_MAX_MANE_LENGHT)
		return (false);
	if (Pname.find(" ") != std::string::npos || Pname.find(",") != std::string::npos)
		return (false);
	if (Pname[0] != '#' || Pname[0] != '&')
		return (false);
	return (true);
}

void Channel::addUser(Client &u) { users.insert(&u); }
void Channel::delUser(Client &u) { users.erase(&u); }

void Channel::opUser(Client &u)
{
	if (users.count(&u))
		op.insert(&u);
}

void Channel::unopUser(Client &u) { op.erase(&u); }