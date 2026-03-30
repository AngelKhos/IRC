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