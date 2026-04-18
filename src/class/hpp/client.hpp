#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

typedef struct Client
{
		Client();
		Client(const Client &src);
		Client(int fd);
		~Client();
		int Recv();
		int Send();
		void Disconnect();
		int client_fd;

		std::string nickName;
		std::string userName;
		std::string ip;

		std::string recv_buff;
		std::string send_buff;
		
		bool has_pass;
		bool has_nick;
		bool has_user;
		bool is_registered;
		bool quit;
} Client;

#endif //CLIENT_HPP

