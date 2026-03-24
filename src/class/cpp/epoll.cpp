#include "epoll.hpp"
#include <unistd.h>

Epoll::Epoll() : fd(-1) {
}

int Epoll::getEpollFd() { return (fd); }
int Epoll::getEventFd(int n) {return (event_list[n].data.fd);}
int Epoll::getEvent(int n) {return (event_list[n].events);}
epoll_event *Epoll::getEventList() {return (event_list);}

void Epoll::setEpollFd(int size)
{
	if (fd != -1)
		close(fd);
	fd = epoll_create(size);
}

int Epoll::ctl_add(int _fd, int opt)
{
	epoll_event event;
	event.events = opt;
	event.data.fd = _fd;
	if (epoll_ctl(fd, EPOLL_CTL_ADD, _fd, &event) == -1)
		throw epollAddErrorException();
	return (true);
}

void Epoll::ctl_mod(int _fd, int opt)
{
	epoll_event event;
	event.events = opt;
	event.data.fd = _fd;
	epoll_ctl(fd, EPOLL_CTL_MOD, _fd, &event);
}
void Epoll::ctl_del(int _fd)
{
	epoll_ctl(fd, EPOLL_CTL_DEL, _fd, NULL);
}

int Epoll::wait()
{
	int ret = epoll_wait(fd, event_list, 10, 10000);
	if (ret == -1)
		throw epollWaitErrorException();
	return (ret);
}

const char *Epoll::epollWaitErrorException::what() const throw()
{
	return ("epoll wait fail");
}

const char *Epoll::epollAddErrorException::what() const throw()
{
	return ("epoll add fail");
}
