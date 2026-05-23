#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <netinet/in.h>
#include "client.hpp"
#include <set>
#include <map>
#include "epoll.hpp"
#include "channel.hpp"
#include "numeric_rpl.hpp"

// Reset
#define ANSI_RESET         "\x1b[0m"

// Regular colors
#define ANSI_BLACK         "\x1b[30m"
#define ANSI_RED           "\x1b[31m"
#define ANSI_GREEN         "\x1b[32m"
#define ANSI_YELLOW        "\x1b[33m"
#define ANSI_BLUE          "\x1b[34m"
#define ANSI_MAGENTA       "\x1b[35m"
#define ANSI_CYAN          "\x1b[36m"
#define ANSI_WHITE         "\x1b[37m"

// Bright colors
#define ANSI_BRIGHT_BLACK      "\x1b[90m"
#define ANSI_BRIGHT_RED        "\x1b[91m"
#define ANSI_BRIGHT_GREEN      "\x1b[92m"
#define ANSI_BRIGHT_YELLOW     "\x1b[93m"
#define ANSI_BRIGHT_BLUE       "\x1b[94m"
#define ANSI_BRIGHT_MAGENTA    "\x1b[95m"
#define ANSI_BRIGHT_CYAN       "\x1b[96m"
#define ANSI_BRIGHT_WHITE      "\x1b[97m"

// Background colors
#define ANSI_BG_BLACK      "\x1b[40m"
#define ANSI_BG_RED        "\x1b[41m"
#define ANSI_BG_GREEN      "\x1b[42m"
#define ANSI_BG_YELLOW     "\x1b[43m"
#define ANSI_BG_BLUE       "\x1b[44m"
#define ANSI_BG_MAGENTA    "\x1b[45m"
#define ANSI_BG_CYAN       "\x1b[46m"
#define ANSI_BG_WHITE      "\x1b[47m"

// Bright background colors
#define ANSI_BG_BRIGHT_BLACK      "\x1b[100m"
#define ANSI_BG_BRIGHT_RED        "\x1b[101m"
#define ANSI_BG_BRIGHT_GREEN      "\x1b[102m"
#define ANSI_BG_BRIGHT_YELLOW     "\x1b[103m"
#define ANSI_BG_BRIGHT_BLUE       "\x1b[104m"
#define ANSI_BG_BRIGHT_MAGENTA    "\x1b[105m"
#define ANSI_BG_BRIGHT_CYAN       "\x1b[106m"
#define ANSI_BG_BRIGHT_WHITE      "\x1b[107m"

// Text styles
#define ANSI_BOLD          "\x1b[1m"
#define ANSI_DIM           "\x1b[2m"
#define ANSI_ITALIC        "\x1b[3m"
#define ANSI_UNDERLINE     "\x1b[4m"
#define ANSI_BLINK         "\x1b[5m"
#define ANSI_REVERSE       "\x1b[7m"
#define ANSI_HIDDEN        "\x1b[8m"
#define ANSI_STRIKE        "\x1b[9m"

#define SERVER_NAME ":kiwi.serv"

typedef struct s_config
{
	unsigned int max_channel_users;
} t_config;

class Server
{
	public:
		Server(unsigned short port, std::string Ppass);
		~Server();

		//exception
		class socketErrorException : public std::exception {
			public:
				virtual const char *what() const throw(); };
		class bindErrorException : public std::exception {
			public:
				virtual const char *what() const throw(); };
		class listenErrorException : public std::exception {
			public:
				virtual const char *what() const throw(); };

		//set/get port
		void setPort(unsigned short Pport);
		unsigned short getPort();

		//set/get password
		void setPassword(std::string Ppass);
		std::string getPassword();

		//set/get server_fd
		void setServerFd(int fd);
		int getServerFd();

		void startServer();
		void stopServer(int x);

		void addChannel(Client &Pclient, std::string Pname);
		void addChannel(std::string Pname);

		
		
		void loop();
		typedef void (Server::*command)(std::vector<std::string>, int);
		std::map<std::string, command> commands;
		void initCommand();

	private:
		
		//commands
		void join(std::vector<std::string> args, int client_fd);
		void nick(std::vector<std::string> args, int client_fd);
		void user(std::vector<std::string> args, int client_fd);
		void pass(std::vector<std::string> args, int client_fd);
		void mode(std::vector<std::string> args, int client_fd);
		void motd(std::vector<std::string> args, int client_fd);
		void quit(std::vector<std::string> args, int client_fd);
		void ping(std::vector<std::string> args, int client_fd);
		void topic(std::vector<std::string> args, int client_fd);
		void invite(std::vector<std::string> args, int client_fd);
		void privmsg(std::vector<std::string> args, int client_fd);
		void kick(std::vector<std::string> args, int client_fd);
		void part(std::vector<std::string> args, int client_fd);

		void connectClient();
		void disconnectClient(int fd);
		void updateClient(int fd, std::string message);
		void registerClient(int fd);

		void processCommand(int fd);
		void sendMsg(int src_fd, int dest_fd, std::string message, std::string command);
		void sendMsginChan(int src_fd, int dest_fd, std::string chName, std::string message, std::string command);

		int server_fd;
		t_config servConfig;
		unsigned short port;
		std::string password;
		bool run;
		sockaddr_in address;
		std::map<int ,Client *> clients; // --> int = client fd, Client = Class client
		Epoll epoll;
		NumRpl Rep;
		std::set<Channel *> channels;
};


std::vector<std::string> cmd_split(std::string args);

#endif //SERVER_HPP