#ifndef EPOLL_H
#define EPOLL_H

#include "apue.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <string>

class EpollServer
{
public:
    EpollServer();
    virtual ~EpollServer();

    bool _listen(const std::string& sPort);
    bool pulse();

private:
    bool setUnblock(int socket);
    bool createEpoll();
    bool addEpoll(int socket, epoll_event& e);
    bool isEpollErr(const epoll_event& e);
    bool isEpollNewConnection(const epoll_event& e);
    bool _error(const epoll_event& e);
    bool _accept(epoll_event& e);
    bool _receive(const epoll_event& e);
    bool _send(int clientFd, const std::string& s);
    bool removeClient(int clientFd);
private:

    addrinfo m_serverAddr;
    int m_listenerSocket;
    int m_epollFd;
    epoll_event m_epollEvent;
    epoll_event* m_pEollEvents;
    char m_readBuffer[1024];
};


#endif
