#include "server.hpp"

void Server::cmd_user(Client c, std::string name, std::string hostname, std::string serverName, std::string realName)
{
	c.userName = name;
	(void)hostname;
	(void)serverName;
	(void)realName;
}