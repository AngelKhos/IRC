#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <netinet/in.h>
#include "client.hpp"
#include <vector>

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
		void stopServer();
		void loop();
	private:
		int server_fd;
		unsigned short port;
		std::string password;
		bool run;
		sockaddr_in address; // TODO set/get for this
		std::vector<Client> clients;
};

#endif //SERVER_HPP