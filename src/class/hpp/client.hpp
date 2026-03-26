#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client
{
	public:
		Client();
		Client(const Client &src);
		Client(int fd);
		~Client();


		//set/get client_fd
		int getClientFd() const;
		void setClientFd(int fd);

		std::string getNickName() const;
		void setNickName(std::string PnickName);

		std::string getName() const;
		void setName(std::string Pname);

		void Disconnect();
	private:
		int client_fd;
		std::string nickName;
		std::string userName;
};

#endif //CLIENT_HPP

