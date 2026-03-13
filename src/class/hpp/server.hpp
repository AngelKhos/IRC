#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>

class Server
{
	public:
		Server(unsigned short port, std::string Ppass);
		~Server();

		void setPort(unsigned short Pport);
		unsigned short getPort();
		void setPassword(std::string Ppass);
		std::string getPassword();
	private:
		unsigned short port;
		std::string password;
};

#endif //SERVER_HPP