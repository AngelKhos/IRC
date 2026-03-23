#ifndef EPOLL_HPP
#define EPOLL_HPP

class Epoll
{
	public:
		Epoll();
		void wait();

		//set/get fd
		int getFd();
		void setFd(int Pfd);

	private:
		int fd;
};

#endif //EPOLL_HPP