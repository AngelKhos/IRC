#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>
#include <set>
#include "client.hpp"

#define CH_MAX_MANE_LENGHT 200

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

		bool checkName(std::string Pname);

		std::string name;
		std::set<Client *> op;
		std::set<Client *> users;
};

#endif //CHANNEL_HPP