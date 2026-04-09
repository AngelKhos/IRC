#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>
#include <set>
#include "client.hpp"

#define CH_MAX_MANE_LENGHT 200

typedef enum e_chModes
{
	modes_i = 0,
	modes_t,
	modes_k,
	modes_o,
	modes_l
} t_chModes;

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

		t_chModes getMode() const;

		static bool checkName(std::string Pname);
	private:
		t_chModes mode;
		std::string topic;
		std::string name;
		std::set<Client *> op;
		std::set<Client *> users;
};

#endif //CHANNEL_PP