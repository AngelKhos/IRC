#include "client.hpp"
#include <iostream>
#include <unistd.h>

//=============================================
//constructor/destructor

Client::Client() :
	client_fd(-1)
{
}

Client::Client(const Client &src)
{
	client_fd = src.getClientFd();
}

Client::Client(int fd) :
	client_fd(fd)
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

std::string Client::getNickName() {return (nickName);}
void Client::setNickName(std::string PnickName) {nickName = PnickName;}

std::string Client::getName() {return (userName);}
void Client::setName(std::string Pname) {userName = Pname;}

//=============================================
//method

void Client::Disconnect()
{
	std::cout << "client disconnected" << std::endl;
	close(client_fd);
}
