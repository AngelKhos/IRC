#ifndef CLIENT_HPP
#define CLIENT_HPP

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

		void Disconnect();
	private:
		int client_fd;
};

#endif //CLIENT_HPP

