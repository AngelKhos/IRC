#include "server.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

//===============================================
//constructor/destructor

Server::Server(unsigned short Pport, std::string Ppass) :
	server_fd(-1),
	port(Pport),
	password(Ppass),
	run(false)
{}

Server::~Server()
{
}

//===============================================
//seter/geter

//Port
void Server::setPort(unsigned short Pport) { port = Pport; }
unsigned short Server::getPort() { return (port); }

//Password
void Server::setPassword(std::string Ppass) { password = Ppass; }
std::string Server::getPassword() { return (password); }

//server_fd
void Server::setServerFd(int fd) { server_fd = fd; }
int Server::getServerFd() { return (server_fd); }

//runing
bool Server::isRuning() { return (run); }

//===============================================
//methode

void Server::startServer()
{
	std::cout << "server stating..." << std::endl;
	this->setServerFd(socket(AF_INET, SOCK_STREAM, 0));
	address.sin_family = AF_INET;
	address.sin_port = htons(this->getPort());
	address.sin_addr.s_addr = INADDR_ANY;

	bind(this->getServerFd(), (sockaddr*)&address, sizeof(address));

	listen(this->getServerFd(), 10);
	run = true;
	std::cout << "the server is on" << std::endl;
}
void Server::stopServer()
{
	close(server_fd);
	run = false;
}