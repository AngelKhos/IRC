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
	client_fd = src.getClientFd();
	userName = src.getName();
	nickName = src.getNickName();
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
//seter/geter

//client_fd
int Client::getClientFd() const { return (client_fd); }
void Client::setClientFd(int fd) { client_fd = fd; }

std::string Client::getNickName() const {return (nickName);}
void Client::setNickName(std::string PnickName) {nickName = PnickName;}

std::string Client::getName() const {return (userName);}
void Client::setName(std::string Pname) {userName = Pname;}

//=============================================
//method

void Client::Disconnect()
{
	std::cout << "client " << client_fd << " disconnected" << std::endl;
	close(client_fd);
}
