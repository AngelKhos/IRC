#include <vector>
#include <string>
#include <iostream>

std::vector<std::string> lil_split(std::string args)
{
	std::vector<std::string> vec;
	for (size_t i = 0; i <= args.size() + 1; i++)
	{

		if (args[i] != ',' && args[i] != '\0')
			continue ;
		else
		{
			size_t j = 0;
			if (args[0] == ',' || args[0] == '\0')
				j++;
			vec.push_back(args.substr(j, i - j));
			args.erase(0, i);
			i = 0;
		}
	}
	return (vec);
}

std::vector<std::string> cmd_split(std::string args)
{
	std::vector<std::string> vec;
	for (size_t i = 0; i < args.size(); ++i)
	{
		if (args[i] == ':')
		{
			vec.push_back(args.substr(i + 1, args.size() - 2));
			args.erase(0, args.size());
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