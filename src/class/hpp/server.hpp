#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <netinet/in.h>

class Server
{
	public:
		Server(unsigned short port, std::string Ppass);
		~Server();


		//set/get port
		void setPort(unsigned short Pport);
		unsigned short getPort();

		//set/get password
		void setPassword(std::string Ppass);
		std::string getPassword();

		//set/get server_fd
		void setServerFd(int fd);
		int getServerFd();

		bool isRuning();

		void startServer();
		void stopServer();
	private:
		int server_fd;
		unsigned short port;
		std::string password;
		bool run;
		sockaddr_in address; // TODO set/get for this
};

#endif //SERVER_HPP