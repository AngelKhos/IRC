#include "server.hpp"
#include <stdlib.h>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <iostream>
#include <sys/epoll.h>

//===============================================
//constructor/destructor

void Server::initCommand()
{
	commands["PASS"] = &Server::pass;
	commands["NICK"] = &Server::nick;
	commands["USER"] = &Server::user;
	commands["JOIN"] = &Server::join;
	commands["MOTD"] = &Server::motd;
	commands["MODE"] = &Server::mode;
	commands["QUIT"] = &Server::quit;
	commands["TOPIC"] = &Server::topic;
	commands["INVITE"] = &Server::invite;
	commands["PRIVMSG"] = &Server::privmsg;
}

Server::Server(unsigned short Pport, std::string Ppass) :
	server_fd(-1),
	port(Pport),
	password(Ppass),
	run(false)
{
	initCommand();
}

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
	for (std::set<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		delete *it;
	}
	for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		disconnectClient(it->second->client_fd);
	}
	clients.clear();

	
	//faudra boucler sur le set des channels pour les del aussi
	if (epoll.getEpollFd() != -1)
		close(epoll.getEpollFd());
	close(server_fd);
	run = false;
}

void Server::connectClient()
{
	struct sockaddr_storage client_addr;
	socklen_t sock_size = sizeof(client_addr);
	int client_fd = accept(this->getServerFd(), (sockaddr *)&client_addr, &sock_size); // accepte les clients qui attendent dans la pool de listen
	if (client_fd != -1)
	{
		Client  *c = new Client(client_fd);
		clients[client_fd] = c; //init client très sommaire (TODO)
		epoll.ctl_add(client_fd, EPOLLIN); //ajoute le client_fd a la liste de fd que epoll doit surveiller

		struct sockaddr_in *sock = (sockaddr_in *)&client_addr;
		clients[client_fd]->ip = inet_ntoa(sock->sin_addr);

		std::cout << "client connected with ip " << clients[client_fd]->ip << std::endl;
	}
}

void Server::disconnectClient(int fd)
{
	epoll.ctl_del(fd);
	clients[fd]->Disconnect();
	delete clients[fd];
	//clients.erase(fd); // cette ligne fesais segfault quand on quittai avec un user dans un channel
}

void Server::updateClient(int fd, std::string message) //fait en sorte que si on a qqch a envoyer, ca set EPOLLOUT
{
	if (clients[fd]->send_buff == "") //si le buffer est vide ca veux dire que yavais rien a envoyer avant
		epoll.ctl_mod(fd, EPOLLIN | EPOLLOUT);
	clients[fd]->send_buff += message;
}

void Server::registerClient(int fd)
{
	std::vector<std::string> null;

	clients[fd]->is_registered = true;
	updateClient(fd, Rep.rpl001("turbo.serv", clients[fd]->nickName));
	motd(null, fd);
}

void Server::processCommand(int fd)
{
	while (clients[fd]->recv_buff.find("\r\n") != std::string::npos)
	{
		std::string message = clients[fd]->recv_buff.substr(0, clients[fd]->recv_buff.find("\r\n") + 1); //la commande
		std::cout << clients[fd]->recv_buff; // logs
		std::vector<std::string> args = cmd_split(message);
		if (!args.empty())
			for (size_t i = 0; i < args[0].length(); i++)
				args[0][i] = toupper(args[0][i]);
		if (!args.empty() && commands.find(args[0]) != commands.end()) //trouver la bonne commande
		{
			std::string command = args[0];
			args.erase(args.begin());
			(this->*commands[command])(args, fd);
		}
		else //command not found
		{
			if (clients[fd]->is_registered)
				updateClient(fd, Rep.err421(args[0], clients[fd]->nickName));
		}
		clients[fd]->recv_buff.erase(0, clients[fd]->recv_buff.find("\r\n") + 2); //enlever la commande qui a ete process
	}
	if (clients[fd]->has_nick && clients[fd]->has_pass && clients[fd]->has_user && !clients[fd]->is_registered) // logging in (USER + PASS + NICK)
		registerClient(fd);
}

void Server::loop()
{
	while (this->run)
	{
		int nb_event = 0;
		try
		{
			nb_event = epoll.wait(); // attends qu'un fd bouge son cul
		}
		catch(const std::exception& e) //si jamais ya un ctrl+c ou un soucis de wait()
		{
			std::cout << "Server internal error: " << e.what() << '\n';
			return ;
		}
		for (int n = 0; n < nb_event; n++) //traiter les events (IN, OUT)
		{
			if (epoll.getEventFd(n) == server_fd) //si c'est le server qui essaye de communiquer ca veut dire que ya un nouveau client dans listen
				connectClient();
			else // si c'est pas serv, ca veut dire client essaye de communiquer
			{
				int bytes_read;
				if (epoll.getEvent(n) & EPOLLIN) //si serveur a recu message de client[fd]
				{
					bytes_read = clients[epoll.getEventFd(n)]->Recv(); //lire ce que client a envoyer
					if (bytes_read == 0)
						disconnectClient(epoll.getEventFd(n));
					else if (clients[epoll.getEventFd(n)]->recv_buff.find("\r\n") != std::string::npos) //si ya un crlf faut process la command (si s'en est une)
						processCommand(epoll.getEventFd(n));
					continue ;
				}

				if (epoll.getEvent(n) & EPOLLOUT) // send message to client that can receive it
				{
					int byte_sent;
					
					byte_sent = clients[epoll.getEventFd(n)]->Send(); //ecrit le buffer d'envoi dans le fd
					if (byte_sent <= 0)
						std::cout << "ah, dommage" << std::endl; //flemme de faire le check faut check errno et tout
					epoll.ctl_mod(epoll.getEventFd(n), EPOLLIN);
				}
				if (clients[epoll.getEventFd(n)]->quit)
				{
					disconnectClient(epoll.getEventFd(n));
				}
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
