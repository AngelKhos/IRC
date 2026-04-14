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

		std::string getTopic() const;
		void setTopic(std::string Ptopic);

		void addUser(Client &u);
		void delUser(Client &u);
		void opUser(Client &u);
		void unopUser(Client &u);

		std::string getName();
		std::set<Client *> &getUsers();
		std::set<Client *> &getOpUsers();

		void setInvOnly(bool inv);
		bool isInvOnly();
		void setLimitUser(int limit);
		int getUserLimit();
		void setPw(std::string pw);
		std::string getPw();

		bool isOp(int client_fd);

		static bool checkName(std::string Pname);
	private:
		std::string password;
		std::string topic;
		std::string name;
		std::set<Client *> op;
		std::set<Client *> users;

		bool invOnly;
		int userLimit;
		bool reTopic;
};

//random utils
Channel *getChannelByName(std::string ch, std::set<Channel *>channels);

#endif //CHANNEL_PP