#include "server.hpp"
#include <vector>
#include <string>

std::vector<std::string> lil_split(std::string args)
{
	std::vector<std::string> vec;
	for (size_t i = 0; i < args.size(); ++i)
	{
		if (args[i] != ',' && i != args.size() - 1)
			continue ;
		else
		{
			size_t j = 0;
			if (args[0] == ',')
				j++;
			vec.push_back(args.substr(j, i - j));
			args.erase(0, i);
			i = 0;
		}
	}
	return (vec);
}

void Server::join(std::vector<std::string> args, int client_fd)
{
	std::vector<std::string> vec_args = cmd_split(args);
	std::vector<std::string> vec_ch = lil_split(vec_args[0]);
	std::vector<std::string> vec_pw = lil_split(vec_args[1]);

	(void)client_fd;
}