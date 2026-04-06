#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <netinet/in.h>
#include "client.hpp"
#include <set>
#include <map>
#include "epoll.hpp"
#include "channel.hpp"
#include "numeric_rpl.hpp"

typedef struct s_config
{
	unsigned int max_channel_users;
} t_config;

class Server
{
	public:
		Server(unsigned short port, std::string Ppass);
		~Server();

		//exception
		class socketErrorException : public std::exception {
			public:
				virtual const char *what() const throw(); };
		class bindErrorException : public std::exception {
			public:
				virtual const char *what() const throw(); };
		class listenErrorException : public std::exception {
			public:
				virtual const char *what() const throw(); };

		//set/get port
		void setPort(unsigned short Pport);
		unsigned short getPort();

		//set/get password
		void setPassword(std::string Ppass);
		std::string getPassword();

		//set/get server_fd
		void setServerFd(int fd);
		int getServerFd();

		bool isRunning();

		void startServer();
		void stopServer(int x);

		void addChannel(Client &Pclient, std::string Pname);
		void addChannel(std::string Pname);

		void processCommand(int fd);
		//void sendMsg(int fd);
		
		void loop();
		typedef void (Server::*command)(std::vector<std::string>, int);
		std::map<std::string, command> commands;
		void initCommand();

	private:
		
		//commands
		void join(std::vector<std::string> args, int client_fd);
		void nick(std::vector<std::string> args, int client_fd);
		void user(std::vector<std::string> args, int client_fd);
		void pass(std::vector<std::string> args, int client_fd);

		void connectClient();
		void disconnectClient(int fd);
		void updateClient(int fd, std::string message);

		int server_fd;
		t_config servConfig;
		unsigned short port;
		std::string password;
		bool run;
		sockaddr_in address; // TODO set/get for this
		std::map<int ,Client *> clients; // --> int = client fd, Client = Class client
		Epoll epoll;
		NumRpl Rep;
		std::set<Channel *> channels;
};


std::vector<std::string> cmd_split(std::string args);

#endif //SERVER_HPP