#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

typedef struct Client
{
		Client();
		Client(const Client &src);
		Client(int fd);
		~Client();
		void Disconnect();
		int client_fd;
		std::string nickName;
		std::string userName;
} Client;

#endif //CLIENT_HPP

