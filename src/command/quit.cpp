#include "server.hpp"

void Server::quit(std::vector<std::string> args, int client_fd)
{
    if (!args.empty())
    {
        std::string rpl = SERVER_NAME + std::string(" QUIT ") + args[0] + std::string(" ; User ") 
        + clients[client_fd]->userName + std::string(" has quit IRC");
        updateClient(client_fd, rpl);
    }
    else
    {
        std::string rpl = SERVER_NAME + std::string(" QUIT ; User ") 
        + clients[client_fd]->userName + std::string(" has quit IRC");
        updateClient(client_fd, rpl);
    }
    clients[client_fd]->quit = true;
}