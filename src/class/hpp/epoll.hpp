#ifndef EPOLL_HPP
#define EPOLL_HPP

#include <sys/epoll.h>
#include <exception>

class Epoll
{
	public:
		Epoll();
		int wait();
		int ctl_add(int _fd, int opt);
		void ctl_mod(int _fd, int opt);
		void ctl_del(int _fd);

		//set/get fd
		int getEpollFd();
		void setEpollFd(int Pfd);

		epoll_event *getEventList();
		int getEventFd(int n);
		int getEvent(int n);

		class epollWaitErrorException : public std::exception {
			public:
				virtual const char *what() const throw(); };
		class epollWaitInterruptException : public std::exception {
			public:
				virtual const char *what() const throw(); };
		class epollAddErrorException : public std::exception {
			public:
				virtual const char *what() const throw(); };

	private:
		int fd;
		epoll_event event_list[10];
};

#endif //EPOLL_HPP