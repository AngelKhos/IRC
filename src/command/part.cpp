#include "server.hpp"

void Server::part(std::vector<std::string> args, int client_fd)
{
    if (args.size() < 1)
        updateClient(client_fd, Rep.err461("PART", clients[client_fd]->nickName));
    else
    {
        std::vector<std::string> target = lil_split(args[0]);

        for (std::vector<std::string>::iterator it = target.begin(); it != target.end(); it++)
        {
            Channel *ch = getChannelByName(*it, channels);
            if (ch)
            {
                std::set<Client *> users = ch->getUsers();
                if (getUserByNick(clients[client_fd]->nickName , users))
                {
                    for (std::set<Client *>::iterator u_it = users.begin(); u_it != users.end(); u_it++)
                    {
                        updateClient( (*u_it)->client_fd, clients[client_fd]->prefix() + std::string(" PART ") 
				                        + *it + std::string("\r\n"));
                    }
                    ch->delUser(*clients[client_fd]);
                    
                }
                else
                    updateClient(client_fd, Rep.err442(*it, clients[client_fd]->nickName));
            }
            else
                updateClient(client_fd, Rep.err403(*it, clients[client_fd]->nickName));
        }
    }
}