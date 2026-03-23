#include "epoll.hpp"
#include <unistd.h>
#include <sys/epoll.h>

Epoll::Epoll() : fd(-1) {}

int Epoll::getFd() { return (fd); }

void Epoll::setFd(int Pfd)
{
	if (fd != -1)
		close(fd);
	fd = Pfd;
}