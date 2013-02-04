#include "epoll.h"
#include <fcntl.h>
#include <errno.h>

#define MAXEVENTS 64

EpollServer::EpollServer():m_pEollEvents(NULL){

}

EpollServer::~EpollServer(){
    if(m_pEollEvents != NULL)
    {
        delete []m_pEollEvents;
        m_pEollEvents = NULL;
    }
}


bool EpollServer::_listen(const std::string& sPort)
{
    printf("try to listen port: %s \n", sPort.c_str());

    addrinfo * pRet = NULL;
    memset((char*)&m_serverAddr, 0, sizeof(m_serverAddr));
    m_serverAddr.ai_family = AF_UNSPEC;
    m_serverAddr.ai_socktype = SOCK_STREAM;
    m_serverAddr.ai_flags = AI_PASSIVE;//*****//

    if(getaddrinfo(NULL, sPort.c_str(), &m_serverAddr, &pRet)!=0)
    {
        err_sys("fail to getaddrinfo");
    }

    if(pRet != NULL)
    {
        for(addrinfo* pRes = pRet ; pRes != NULL; pRes = pRes->ai_next)
        {
            if((m_listenerSocket = socket(pRes->ai_family, pRes->ai_socktype, pRes->ai_protocol)) < 0)
                err_sys("failed to create socket");
            if(bind(m_listenerSocket, pRes->ai_addr, pRes->ai_addrlen) == -1)
            {
                close(m_listenerSocket);
                err_sys("error bind, %d,%d" ,pRes->ai_family ,pRes->ai_addr);
            }
            freeaddrinfo(pRet);
            setUnblock(m_listenerSocket);
            if(listen(m_listenerSocket, SOMAXCONN) <0 )
                err_sys("listen socket");
            printf("listen ok");
            return createEpoll();
        }
    }

    return false;
}

bool EpollServer::pulse()
{
    int n = epoll_wait(m_epollFd, m_pEollEvents, MAXEVENTS, -1);
    for(int i = 0; i< n;i ++)
    {
        epoll_event& e = m_pEollEvents[i];
        if(isEpollErr(e))
            _error(e);
        else if( isEpollNewConnection(e))
        {
            _accept(e);
        }
        else
        {
            _receive(e);
        }
    }
    return true;
}


bool EpollServer::setUnblock(int socket)
{

    int flag = 0;
    if((flag = fcntl(socket, F_GETFL, 0)) != -1)
    {
        flag |= O_NONBLOCK;
        if(fcntl(socket, F_SETFL, flag ) != -1)
            return true;
    }
    err_sys("falied to call fcntl F_SETFL for m_listenerSocket");
    return false;
}

bool EpollServer::createEpoll()
{
    printf("try to create poll\n");
    if( (m_epollFd = epoll_create1(0)) == -1)
        err_sys("fail to create epoll");
    m_epollEvent.data.fd = m_listenerSocket;
    m_epollEvent.events = EPOLLIN | EPOLLET;
    if(addEpoll(m_listenerSocket, m_epollEvent))
    {
        m_pEollEvents = new epoll_event[MAXEVENTS];
        printf("create poll ok\n");
        return true;
    }
    return false;
}

bool EpollServer::addEpoll(int socket, epoll_event& e)
{
    if(epoll_ctl(m_epollFd, EPOLL_CTL_ADD, socket, &e) < 0){
        err_msg("call poll epoll_ctl ok");
        return false;
    }
    return true;
}

bool EpollServer::isEpollErr(const epoll_event& e)
{
    return ((e.events& EPOLLERR) || (e.events & EPOLLHUP) || !(e.events & EPOLLIN));
}


bool EpollServer::isEpollNewConnection(const epoll_event& e)
{
    return m_listenerSocket == e.data.fd;
}

bool EpollServer::_error(const epoll_event& e)
{
    err_msg("error epoll client");
    removeClient(e.data.fd);
}

bool EpollServer::_accept(epoll_event& e)
{
    printf("a new connect is comming :%d\n", e.data.fd);
    sockaddr clientAddr;
    int clientFd = 0;
    socklen_t clientAddrLen = sizeof(clientAddr);
    char hBuf[NI_MAXHOST], sBuf[NI_MAXSERV];

    if((clientFd = accept(m_listenerSocket, &clientAddr, &clientAddrLen)) == -1)
    {
        if((errno != EAGAIN ) && (errno != EWOULDBLOCK))
        {
            err_msg("failed to accept new client");
            return false;
        }
    }

    if(getnameinfo(&clientAddr, clientAddrLen, hBuf, sizeof(hBuf), sBuf, sizeof(sBuf), NI_NUMERICHOST | NI_NUMERICSERV) <0)
    {
        err_msg("failed to get name info ");
    }

    if(!setUnblock(e.data.fd))
    {
        err_msg("faild to set unblock fd ");
        return false;
    }

    e.data.fd = clientFd;
    e.events = EPOLLIN | EPOLLET;
    return addEpoll(clientFd, e);
}


bool EpollServer::_receive(const epoll_event &e)
{

    int clientFd = e.data.fd;
    uint32_t nbytes = recv(clientFd, m_readBuffer, sizeof(m_readBuffer), 0);
    printf("receive %d msgs from clientfd:%s", nbytes, m_readBuffer);
    if(nbytes > 0)
    {
        std::string sData(m_readBuffer, nbytes);
        _send(1,sData);
        _send(clientFd, sData);
    }
    else
    {
        err_sys("nbyes wrong");
        removeClient(clientFd);
    }
    return true;
}


bool EpollServer::_send(int clientFd, const std::string &sData)
{
    if(write(clientFd, sData.c_str(), sData.size()) < 0){
        err_msg("write error");
        return false;
    }
    return true;
}

bool EpollServer::removeClient(int clientFd)
{
    printf("try to remove clientFd");
    close(clientFd);
    return true;
}
