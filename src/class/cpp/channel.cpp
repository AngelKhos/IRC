#include "channel.hpp"

Channel::Channel() : name("") {}

Channel::Channel(std::string Pname) : name(Pname) {}

Channel::~Channel() {}


void Channel::addUser(Client &u) { users.insert(&u); }
void Channel::delUser(Client &u) { users.erase(&u); }

void Channel::opUser(Client &u) {}
void Channel::unopUser(Client &u) {}