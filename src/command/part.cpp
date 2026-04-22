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
                if (getUserByNick(clients[client_fd]->nickName , ch->getUsers()))
                {
                    ch->delUser(*clients[client_fd]);

                    std::string rpl = clients[client_fd]->prefix() + std::string(" PART ") 
				                    + *it + "\r\n";
                    updateClient(client_fd, rpl);
                }
                else
                    updateClient(client_fd, Rep.err442(*it, clients[client_fd]->nickName));
            }
            else
                updateClient(client_fd, Rep.err403(*it, clients[client_fd]->nickName));
        }
    }
}