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
	this->stopServer();
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

//running
bool Server::isRunning() { return (run); }

//===============================================
//method

void Server::startServer()
{
	std::cout << "server starting..." << std::endl;
	this->setServerFd(socket(AF_INET, SOCK_STREAM, 0));
	if (server_fd == -1)
		throw socketErrorException();
	address.sin_family = AF_INET;
	address.sin_port = htons(this->getPort());
	address.sin_addr.s_addr = INADDR_ANY;
	
	if (bind(this->getServerFd(), (sockaddr*)&address, sizeof(address)) == -1)
		throw bindErrorException();

	if (listen(this->getServerFd(), 10) == -1)
		throw listenErrorException();
	run = true;
	std::cout << "the server is on" << std::endl;
}
void Server::stopServer()
{
	for (unsigned long int i = 0; i < clients.size(); i++)
	{
		Client &c = clients[i];
		close(c.getClientFd());
	}
	close(server_fd);
	run = false;
}

void Server::loop()
{
	while (this->isRunning())
	{
		int client_fd = accept(this->getServerFd(), NULL, NULL);
		if (client_fd != -1)
		{
			Client c(client_fd);
			clients.push_back(c);
			std::cout << "client connected" << std::endl;
		}
	}
}

const char *Server::socketErrorException::what() const throw()
{
	return ("socket fail");
}
const char *Server::bindErrorException::what() const throw()
{
	return ("bind fail");
}
const char *Server::listenErrorException::what() const throw()
{
	return ("listen fail");
}