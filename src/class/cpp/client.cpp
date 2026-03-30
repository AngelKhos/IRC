#include "client.hpp"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>

//=============================================
//constructor/destructor

Client::Client() :
	client_fd(-1),
	nickName("user"),
	userName("user"),
	has_pass(false),
	has_nick(false),
	has_user(false)
{}

Client::Client(const Client &src)
{
	client_fd = src.client_fd;
	userName = src.userName;
	nickName = src.nickName;
	has_pass = src.has_pass;
	has_nick = src.has_nick;
	has_user = src.has_user;
}

Client::Client(int fd) :
	client_fd(fd),
	nickName("user"),
	userName("user"),
	has_pass(false),
	has_nick(false),
	has_user(false)
{}

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

int Client::Recv()
{
	char buff[512];
	int read_size;

	read_size = recv(client_fd, buff, 512, 0);
	if (read_size <= 0)
		return (read_size);
	buff[read_size] = '\0';
	recv_buff += buff;

	return read_size;
}

int Client::Send()
{
	int sent_bytes;

	sent_bytes = send(client_fd, send_buff.c_str(), send_buff.size(), 0);
	if (sent_bytes <= 0)
		return (sent_bytes);
	send_buff.erase(0, sent_bytes);
	
	return (sent_bytes);
}
