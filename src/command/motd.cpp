#include "server.hpp"

void Server::motd(std::vector<std::string> args, int client_fd)
{
	(void)args;
	updateClient(client_fd, Rep.rpl375(clients[client_fd]->nickName));
    updateClient(client_fd, Rep.rpl372(clients[client_fd]->nickName));
    updateClient(client_fd, Rep.rpl376(clients[client_fd]->nickName));
}