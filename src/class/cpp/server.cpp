#include "server.hpp"
#include <stdlib.h>
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


	epoll.setFd(epoll_create(1024)); //epoll create initial, notre bébé
	epoll_event ep_event;
	ep_event.events = EPOLLIN; // on a seulement besoin de le IN
	ep_event.data.fd = server_fd; // cet event appartient a serveur
	if (epoll_ctl(epoll.getFd(), EPOLL_CTL_ADD, server_fd, &ep_event) == -1) //ajoute le server_fd a la liste de fd que epoll doit surveiller
		throw epollAddErrorException();

	std::cout << "the server is on" << std::endl;
}
void Server::stopServer()
{
	// for (unsigned long int i = 0; i < clients.size(); i++)
	// {
	// 	Client &c = clients[i];
	// 	close(c.getClientFd());
	// }
	clients.clear();
	//faudra boucler sur la map pour delete les clients
	if (epoll.getFd() != -1)
		close(epoll.getFd());
	close(server_fd);
	run = false;
}

void Server::loop()
{
	while (this->isRunning())
	{
		epoll_event ep_events[10]; // 10 event traité a la fois par boucle (maybe a changer?)

		const int nb_event = epoll_wait(epoll.getFd(), ep_events, 10, 0); // attends qu'un fd bouge son cul (0 pour non-bloquant)
		//faut faire un truc si epoll_wait foire
		for (int n = 0; n < nb_event; n++)
		{
			if (ep_events[n].data.fd == server_fd) //si c'est le server qui essaye de communiquer ca veut dire que ya un nouveau client dans listen
			{
				epoll_event ev;
				ev.events = EPOLLIN | EPOLLOUT; //client_fd en io
				
				int client_fd = accept(this->getServerFd(), NULL, NULL); // accepte les clients qui attendent dans la pool de listen
				if (client_fd != -1)
				{
					ev.data.fd = client_fd;
					Client  *c = new Client(client_fd);
					//sorry, faudra delete
					clients[client_fd] = c; //init client très sommaire (TODO)
					if (epoll_ctl(epoll.getFd(), EPOLL_CTL_ADD, client_fd, &ev) == -1) //ajoute le client_fd a la liste de fd que epoll doit surveiller
						throw epollAddErrorException();
					std::cout << "client connected" << std::endl;
				}
			}
			else // si c'est pas serv, ca veut dire client et la c'est full roue libre
			{
				char buff[4096];
				int read_size = 0;
				if (ep_events[n].events & EPOLLIN) //si serveur a recu mesage de client[id = fd]
				{
					std::cout << "client " << ep_events[n].data.fd << " tried to communicate" << std::endl;
					read_size = recv(ep_events[n].data.fd, buff, 4096, 0); //meme que read mais pour les sockets (rtfm)
					//si -1 ca a pété
					if (read_size == 0) // client a disconnect
					{
						epoll_ctl(epoll.getFd(), EPOLL_CTL_DEL, ep_events[n].data.fd, NULL);
						clients[ep_events[n].data.fd]->Disconnect();
						delete clients[ep_events[n].data.fd];
						clients.erase(ep_events[n].data.fd);
					}
					else // traitement du msg
						std::cout << buff << std::endl;
				}
				if (ep_events[n].events & EPOLLOUT && read_size != 0)
				{
					std::cout << "client " << ep_events[n].data.fd << "is waiting for reply" << std::endl;
					send(ep_events[n].data.fd, buff, read_size, 0);
				}
				//io test for epoll event reception
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

const char *Server::epollAddErrorException::what() const throw()
{
	return ("epoll add fail");
}