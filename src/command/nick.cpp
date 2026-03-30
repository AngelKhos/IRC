#include "server.hpp"

void Server::cmd_nick(Client c, std::string nick)
{
	c.nickName = nick;
}