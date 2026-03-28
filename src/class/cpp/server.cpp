#include "server.hpp"
#include <stdlib.h>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <sys/epoll.h>

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
	this->stopServer(0);
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

void Server::addChannel(Client &Pclient, std::string Pname)
{
	Channel *c = new Channel(Pname);
	c->addUser(Pclient);
	c->opUser(Pclient);
	channels.insert(c);
}

void Server::addChannel(std::string Pname)
{
	Channel *c = new Channel(Pname);
	channels.insert(c);
}

void Server::startServer()
{
	std::cout << "server starting..." << std::endl;
	this->setServerFd(socket(AF_INET, SOCK_STREAM, 0)); //crée le fd pour communiquer avec qqn de distant
	// AF_INET --> address family connection reseau | SOCK_STREAM --> connexion TCP
	if (server_fd == -1)
		throw socketErrorException();
	address.sin_family = AF_INET;
	address.sin_port = htons(this->getPort()); // normalise le port (bon endian)
	address.sin_addr.s_addr = INADDR_ANY;
	
	if (bind(this->getServerFd(), (sockaddr*)&address, sizeof(address)) == -1) //ce socket ecoute ce port la
		throw bindErrorException();

	if (listen(this->getServerFd(), 1024) == -1) //crée une pool d'utilisateur (1024 c'est le nombrre d'utilisateur en attente)
		throw listenErrorException();
	run = true;

	epoll.setEpollFd(1024); //epoll create
	epoll.ctl_add(server_fd, EPOLLIN);
	std::cout << "the server is on" << std::endl;
}
void Server::stopServer(int x)
{
	std::cout << "server off :(((((" << std::endl;
	(void)x;
	// for (unsigned long int i = 0; i < clients.size(); i++)
	// {
	// 	Client &c = clients[i];
	// 	close(c.getClientFd());
	// }
	clients.clear();
	//faudra boucler sur la map pour delete les clients
	//et sur le set des channels pour les del aussi
	if (epoll.getEpollFd() != -1)
		close(epoll.getEpollFd());
	close(server_fd);
	run = false;
}

void Server::connectClient()
{
	int client_fd = accept(this->getServerFd(), NULL, NULL); // accepte les clients qui attendent dans la pool de listen
	if (client_fd != -1)
	{
		Client  *c = new Client(client_fd);
		clients[client_fd] = c; //init client très sommaire (TODO)
		epoll.ctl_add(client_fd, EPOLLIN | EPOLLOUT); //ajoute le client_fd a la liste de fd que epoll doit surveiller
		std::cout << "client connected" << std::endl;
	}
}

void Server::disconnectClient(int fd)
{
	epoll.ctl_del(fd);
	clients[fd]->Disconnect();
	delete clients[fd];
	clients.erase(fd);
}

int Server::recvMsg(int fd)
{
	char buff[512];
	int read_size;

	bzero(buff, 512);
	while (clients[fd]->recv_buff.find("\r\n") == std::string::npos)
	{
		read_size = recv(fd, buff, 512, 0);
		clients[fd]->recv_buff += buff;
	}
	bzero(buff, 512);
	return read_size;
}

// int Server::tokenizeMsg(int fd)
// {
// 	clients[fd]->recv_buff
// }

// void Server::sendMsg(int fd)
// {
	
// }

void Server::loop()
{
	while (this->isRunning())
	{
		const int nb_event = epoll.wait(); // attends qu'un fd bouge son cul
		
		for (int n = 0; n < nb_event; n++)
		{
			if (epoll.getEventFd(n) == server_fd) //si c'est le server qui essaye de communiquer ca veut dire que ya un nouveau client dans listen
				connectClient();
			else // si c'est pas serv, ca veut dire client et la c'est full roue libre
			{
				int oread;
				if (epoll.getEvent(n) & EPOLLIN) //si serveur a recu mesage de client[id = fd]
				{
					oread = recvMsg(epoll.getEventFd(n));
					if (oread == 0)
						disconnectClient(epoll.getEventFd(n));
					else
					{ 
						std::cout << clients[epoll.getEventFd(n)]->recv_buff << std::endl;
						clients[epoll.getEventFd(n)]->recv_buff = "";
					}

				}
				if (clients[epoll.getEventFd(n)]->send_buff.find("USER", 0) == 0)
				{
				
				}
				else if (clients[epoll.getEventFd(n)]->send_buff.find("PRIVMSG", 0) == 0)
				{

				}
				// if (epoll.getEvent(n) & EPOLLOUT)
				// {
				// 	std::cout << "client " << epoll.getEventFd(n) << " is waiting for reply" << std::endl;
				// 	//send(epoll.getEventFd(n), buff, read_size, 0);
				// }
				//io test for epoll event reception

				//TODO irc routine for clients (JOIN general, broadcast msg)
			}
		}
	}
}

//===============================================
//exception

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
