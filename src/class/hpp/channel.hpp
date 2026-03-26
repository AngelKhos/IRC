#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>
#include <set>
#include "client.hpp"

class Channel
{
	public:
		Channel();
		Channel(std::string Pname);
		~Channel();

		void addUser(Client &u);
		void delUser(Client &u);
		void opUser(Client &u);
		void unopUser(Client &u);
	private:
		std::string name;
		std::set<Client *> op;
		std::set<Client *> users;
};

#endif //CHANNEL_HPP