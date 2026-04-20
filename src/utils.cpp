#include <vector>
#include <string>
#include <iostream>

std::vector<std::string> cmd_split(std::string args)
{
	std::vector<std::string> vec;
	for (size_t i = 0; i < args.size(); ++i)
	{
		if (args[i] == ':')
		{
			args.erase(0, 2);
			vec.push_back(args);
			return (vec);
		}
		else
		{
			if (args[i] != ' ' && i != args.size() - 1)
				continue ;
			else
			{
				size_t j = 0;
				if (args[0] == ' ')
					j++;
				vec.push_back(args.substr(j, i - j));
				args.erase(0, i);
				i = 0;
			}
		}
	}
	return (vec);
}