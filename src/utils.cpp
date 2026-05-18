#include <vector>
#include <string>
#include <iostream>

std::vector<std::string> lil_split(std::string args)
{
	std::vector<std::string> vec;
	while (args.find(',') != std::string::npos)
	{
		vec.push_back(args.substr(0, args.find(',')));
		args.erase(0, args.find(',') + 1);
	}
	if (args.size() > 0)
		vec.push_back(args);
	return (vec);
}

std::vector<std::string> cmd_split(std::string args)
{
	std::vector<std::string> vec;
	std::string longArg;
	if (args.find(':') != std::string::npos)
	{
		longArg = args.substr(args.find(':') + 1, args.size() - 1);
		args.erase(args.find(':'), args.size() - 1);
	}

	while (args.find(' ') != std::string::npos)
	{
		vec.push_back(args.substr(0, args.find(' ')));
		args.erase(0, args.find(' ') + 1);
	}
	if (args.size() > 0)
		vec.push_back(args);
	if (!longArg.empty())
		vec.push_back(longArg);
	return (vec);
}