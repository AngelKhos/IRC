#include <server.hpp>

void Server::pass(std::string args, int client_fd)
{
    //if (clients[client_fd]->has_pass == true)
        //462
    //if (args == "\r\n")
        //461
    if (args == password)
        clients[client_fd]->has_pass = true;

    //si le password est invalide, c'est ignoré
}

void Server::cmd_join(Client c, std::string ChName)
{
	
}

void Server::cmd_nick(Client c, std::string nick)
{
	c.nickName = nick;
}

void Server::cmd_user(Client c, std::string name)
{
	c.userName = name;
}