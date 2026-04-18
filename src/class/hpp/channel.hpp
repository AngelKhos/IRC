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
		void setName(std::string Pname);
		std::set<Client *> &getUsers();
		std::set<Client *> &getOpUsers();

		void setInvOnly(bool inv);
		bool isInvOnly();
		void setLimitUser(int limit);
		int getUserLimit();
		void setPw(std::string pw);
		std::string getPw();
		void setReTopic(bool re);
		bool getReTopic();
		void setTopicAutor(std::string nick);
		std::string getTopicAutor();
		void setTopicUpdate(long int time);
		long int getTopicUpdate();
		std::set<std::string> &getInvList();
		bool isInv(Client &client);

		bool isOp(int client_fd);

		static bool checkName(std::string Pname);
	private:
		std::string password;
		std::string topic;
		std::string name;
		std::set<Client *> op;
		std::set<Client *> users;
		std::set<std::string> invList;

		bool invOnly;
		int userLimit;
		bool reTopic;

		std::string topicAutor;
		long int topicUpdate;
};

//random utils
Channel	*getChannelByName(std::string ch, std::set<Channel *>channels);
Client	*getUserByNick(std::string nick, std::set<Client *>Clients);
Client *getUserByNickInMap(std::string nick, std::map<int, Client *>Clients);
std::vector<std::string> lil_split(std::string args);

#endif //CHANNEL_PP