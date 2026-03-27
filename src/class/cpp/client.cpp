#include "client.hpp"
#include <iostream>
#include <unistd.h>

//=============================================
//constructor/destructor

Client::Client() :
	client_fd(-1),
	nickName("user"),
	userName("user")
{
}

Client::Client(const Client &src)
{
	client_fd = src.client_fd;
	userName = src.userName;
	nickName = src.nickName;
}

Client::Client(int fd) :
	client_fd(fd),
	nickName("user"),
	userName("user")
{
}

Client::~Client()
{
}

//=============================================
//method

void Client::Disconnect()
{
	std::cout << "client " << client_fd << " disconnected" << std::endl;
	close(client_fd);
}
