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
	commands["PING"] = &Server::ping;
	commands["TOPIC"] = &Server::topic;
	commands["INVITE"] = &Server::invite;
	commands["PRIVMSG"] = &Server::privmsg;
	commands["KICK"] = &Server::kick;
	commands["PART"] = &Server::part;
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
//setter/getter

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

	//create the socket fd to communicate with distant client
	this->setServerFd(socket(AF_INET, SOCK_STREAM, 0)); 
	// AF_INET --> address family network connection | SOCK_STREAM --> TCP connexion
	if (server_fd == -1)
		throw socketErrorException();

	address.sin_family = AF_INET;
	// normalize the port's endian to standard internet protocol endian
	address.sin_port = htons(this->getPort()); 
	address.sin_addr.s_addr = INADDR_ANY;

	int reuse = 1;
	// set the port so it can be reused by another program anytime
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)))
		throw bindErrorException();
	//links the socket to the port
	if (bind(this->getServerFd(), (sockaddr*)&address, sizeof(address)) == -1) 
		throw bindErrorException();

	//create a 1024 sized user pool
	if (listen(this->getServerFd(), 1024) == -1) 
		throw listenErrorException();

	//epoll create
	epoll.setEpollFd(1024); 
	//adds the server fd to the lists of fds epoll needs to watch so clients can connect
	epoll.ctl_add(server_fd, EPOLLIN);

	run = true;
	std::cout << "Server started!" << std::endl;
}
void Server::stopServer(int )
{
	for (std::set<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		delete *it;
	}

	for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		disconnectClient(it->second->client_fd);
	}

	clients.clear();
	if (epoll.getEpollFd() != -1)
		close(epoll.getEpollFd());

	close(server_fd);

	run = false;
	std::cout << "Server off" << std::endl;
}

void Server::connectClient()
{
	struct sockaddr_storage client_addr;
	socklen_t sock_size = sizeof(client_addr);

	// accepts the clients waiting in the listen pool
	int client_fd = accept(this->getServerFd(), (sockaddr *)&client_addr, &sock_size); 

	if (client_fd != -1)
	{
		Client  *c = new Client(client_fd);
		clients[client_fd] = c;
		//add the client's fd to the pool of fds epoll needs to watch
		epoll.ctl_add(client_fd, EPOLLIN); 

		//get the client's ip
		struct sockaddr_in *sock = (sockaddr_in *)&client_addr;
		clients[client_fd]->ip = inet_ntoa(sock->sin_addr);

		std::cout << "client connected with ip " << clients[client_fd]->ip << std::endl;
	}
}

void Server::disconnectClient(int fd)
{
	std::set<std::string> setChannelRegCpy = clients[fd]->regChannel;
	for (std::set<std::string>::iterator it = setChannelRegCpy.begin(); it != setChannelRegCpy.end(); it++)
	{
		Channel *ch = getChannelByName(*it, channels);
		ch->delUser(*clients[fd]);
	}
	epoll.ctl_del(fd);
	clients[fd]->Disconnect();
	delete clients[fd];
}

void Server::updateClient(int fd, std::string message) 
{
	//if the buffer was empty it means there was nothing to send before
	if (clients[fd]->send_buff == "") 
	{
		//and so, we need to set EPOLLOUT
		epoll.ctl_mod(fd, EPOLLIN | EPOLLOUT);
	}
	clients[fd]->send_buff += message;
}

void Server::registerClient(int fd)
{
	std::vector<std::string> null;

	clients[fd]->is_registered = true;
	updateClient(fd, Rep.rpl001("kiwi.serv", clients[fd]->nickName));
	motd(null, fd);
}

void Server::processCommand(int fd)
{
	while (clients[fd]->recv_buff.find("\r\n") != std::string::npos)
	{
		//the command
		std::string message = clients[fd]->recv_buff.substr(0, clients[fd]->recv_buff.find("\r\n") + 1);

		// logs
		std::cout << clients[fd]->recv_buff; 

		std::vector<std::string> args = cmd_split(message);
		if (!args.empty())
		{
			for (size_t i = 0; i < args[0].length(); i++)
				args[0][i] = toupper(args[0][i]);
		}

		//finding the command in the command list (function pointer)
		if (!args.empty() && commands.find(args[0]) != commands.end()) 
		{
			std::string command = args[0];
			args.erase(args.begin());
			(this->*commands[command])(args, fd);
		}

		//command not found
		else if (args[0] != "") 
		{
			if (clients[fd]->is_registered)
				updateClient(fd, Rep.err421(args[0], clients[fd]->nickName));
		}

		//remove the command that has been processed
		clients[fd]->recv_buff.erase(0, clients[fd]->recv_buff.find("\r\n") + 2); 
	}
	
	// logging in (USER + PASS + NICK)
	if (clients[fd]->has_nick && clients[fd]->has_pass && clients[fd]->has_user && !clients[fd]->is_registered) 
		registerClient(fd);
}

void Server::loop()
{
	while (this->run)
	{
		int nb_event = 0;
		try
		{
			// wait for an fd event
			nb_event = epoll.wait(); 
		}
		//if there is an interrupt (ctrl+c) or a wait error
		catch(const std::exception& e) 
		{
			std::cout << "Server internal error: " << e.what() << '\n';
			return ;
		}

		//event processing (IN, OUT)
		for (int n = 0; n < nb_event; n++) 
		{
			//if the server tries to communicate, there's a client in the listen pool
			if (epoll.getEventFd(n) == server_fd) 
				connectClient();

			// a client tries to communicate
			else 
			{
				int bytes_read;
				//client[fd] tries to send a message to us
				if (epoll.getEvent(n) & EPOLLIN) 
				{
					//reading what the client has to say
					bytes_read = clients[epoll.getEventFd(n)]->Recv(); 
					if (bytes_read == 0)
					{
						disconnectClient(epoll.getEventFd(n));
						clients.erase(epoll.getEventFd(n));
					}
					
					//if there is a crlf, it means its a full "command" in need of processing
					else if (clients[epoll.getEventFd(n)]->recv_buff.find("\r\n") != std::string::npos) 
						processCommand(epoll.getEventFd(n));
					continue ;
				}

				// we need to send a message to client
				if (epoll.getEvent(n) & EPOLLOUT) 
				{
					int byte_sent;
					//sends the message to the client
					byte_sent = clients[epoll.getEventFd(n)]->Send(); 
					if (byte_sent <= 0)
					{
						disconnectClient(epoll.getEventFd(n));
						clients.erase(epoll.getEventFd(n));
					}

					//we have nothing else to say so we set it back to epollin only
					else
						epoll.ctl_mod(epoll.getEventFd(n), EPOLLIN);
				}
				if (clients[epoll.getEventFd(n)]->quit)
				{
					disconnectClient(epoll.getEventFd(n));
					clients.erase(epoll.getEventFd(n));
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
