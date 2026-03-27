#include "channel.hpp"
#include "client.hpp"

//============================================
//constructor//destructor

Channel::Channel() : name("") {}

Channel::Channel(std::string Pname) : name(Pname) {}

Channel::~Channel() {}

//============================================
//seter/geter

std::string Channel::getTopic() const { return (topic); }
void Channel::setTopic(std::string Ptopic) { topic = Ptopic; }

//============================================
//methode

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

//add user to the channel
void Channel::addUser(Client &u) { users.insert(&u); }
//delete a user from the channel
void Channel::delUser(Client &u) { users.erase(&u); }

//make an operator
void Channel::opUser(Client &u)
{
	if (users.count(&u))
		op.insert(&u);
}

//unop a operator in the channel
void Channel::unopUser(Client &u) { op.erase(&u); }