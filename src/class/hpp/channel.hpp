#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>
#include "user.hpp"

class Channel
{
	public:
		Channel();
		Channel(std::string Pname);
		~Channel();

		void addUser(User &u);
		void delUser(int fd);
	private:
		std::string name;
		std::vector<int> clientFds;
};

#endif //CHANNEL_HPP