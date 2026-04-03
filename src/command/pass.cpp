#include <server.hpp>
#include <iostream>

void Server::pass(std::vector<std::string> args, int client_fd)
{
    if (args.empty())
    {
        updateClient(client_fd, Rep.err461("PASS", clients[client_fd]->nickName));
        return ;
    }
    if (clients[client_fd]->has_pass == true)
    {
        updateClient(client_fd, Rep.err462(clients[client_fd]->nickName));
        return ;
    }
    if (args[0] == password)
        clients[client_fd]->has_pass = true;

    //si le password est invalide, c'est ignoré
}