#include "numeric_rpl.hpp"
#include <sys/socket.h>
#include <server.hpp>

void NumRpl::rpl001(int fd, str servName, str nick)
{
    str rpl = str("welcome to the ") +
        servName +  str(" Internet Relay Chat Network ") +
        str(" ") + nick;
    send(fd, rpl.c_str(), rpl.size(), 0);
}

void NumRpl::err421(int fd, str com)
{
    str rpl = com + str(" :Unknow command");
    send(fd, rpl.c_str(), rpl.size(), 0);
}