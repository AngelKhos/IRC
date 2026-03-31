#include <vector>
#include <string>
#include <iostream>

std::vector<std::string> cmd_split(std::string args)
{
	std::vector<std::string> vec;
	for (size_t i = 0; i < args.size(); i++)
	{

		if (args[i] != ':')
		{
			if (args[i] != ' ' && i != args.size() - 1)
				continue ;
			else
			{
				if (args[i] != '\0')
				    i++;
				vec.push_back(args.substr(0, i));
				args.erase(0, i);
				i = 0;
			}
		}
		else
		{
			vec.push_back(args);
			return (vec);
		}
	}
	return (vec);
}
