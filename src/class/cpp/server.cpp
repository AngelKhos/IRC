#include "server.hpp"
#include <iostream>
Server::Server(unsigned short Pport, std::string Ppass) :
	port(Pport),
	password(Ppass)
{}

Server::~Server()
{
}

void Server::setPort(unsigned short Pport) { port = Pport; }

unsigned short Server::getPort() { return (port); }

void Server::setPassword(std::string Ppass) { password = Ppass; }

std::string Server::getPassword() { return (password); }
