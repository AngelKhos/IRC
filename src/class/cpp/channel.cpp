#include "channel.hpp"
#include "client.hpp"

//============================================
//constructor//destructor

Channel::Channel() : password(""), name(""), invOnly(false) {}

Channel::Channel(std::string Pname) : name(Pname) {}

Channel::~Channel() {}

//============================================
//seter/geter

std::string Channel::getTopic() const { return (topic); }
void Channel::setTopic(std::string Ptopic) { topic = Ptopic; }

std::string Channel::getName() { return (name); }

std::set<Client *> &Channel::getUsers() { return (users); }
std::set<Client *> &Channel::getOpUsers() { return (op); }

void Channel::setInvOnly(bool inv) { invOnly = inv; }
bool Channel::isInvOnly() { return (invOnly); }

//============================================
//methode

//check if the name have max of 200 char, # or & at the begin and no space or ','
bool Channel::checkName(std::string Pname)
{
	if (Pname.length() > CH_MAX_MANE_LENGHT)
		return (false);
	if (Pname.find(" ") != std::string::npos && Pname.find(",") != std::string::npos)
		return (false);
	if (Pname[0] != '#' && Pname[0] != '&')
		return (false);
	return (true);
}

bool Channel::isOp(int client_fd)
{
	for (std::set<Client *>::iterator it = op.begin(); it != op.end(); it++)
	{
		if (client_fd == (*it)->client_fd)
			return (true);
	}
	return (false);
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