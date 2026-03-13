#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>

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
	private:
		int server_fd;
		unsigned short port;
		std::string password;
};

#endif //SERVER_HPP