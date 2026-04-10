#include "server.hpp"
#include <vector>
#include "channel.hpp"

void Server::mode(std::vector<std::string> args, int client_fd)
{
	std::string mode = args[1];
	std::string target = args[0];
	Channel *ch = getChannelByName(target, channels);
	if (!ch)
		return ;
	if (!ch->isOp(client_fd))
	{
		updateClient(client_fd, Rep.err482(*ch, clients[client_fd]->nickName));
		return ;
	}

	if (mode == "-i")
	{
		if (args.size() > 2)
			return ;
		
	}
	else if (mode == "+i")
	{}
	else if (mode == "-t")
	{}
	else if (mode == "+t")
	{}
	else if (mode == "-k")
	{}
	else if (mode == "+k")
	{}
	else if (mode == "-o")
	{}
	else if (mode == "+o")
	{}
	else if (mode == "-l")
	{}
	else if (mode == "+l")
	{}
}