#include "select.h"
#include <sys/select.h>

using namespace std;


SelectServer::SelectServer(){
    FD_ZERO(&m_masterFdSet);
    FD_ZERO(&m_readFdSet);
}



bool SelectServer::init(){


    if((m_listenerSocket = socket(AF_INET, SOCK_STREAM, 0)) <0)
        err_sys("socket error , failed to get listening socket");
    uint32_t yes = 0;
    if(setsockopt(m_listenerSocket,SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))<0)
        err_sys("setsockopt eror");
    return true;
}


bool SelectServer::_listen(uint32_t port)
{
    m_serverAddr.sin_family = AF_INET;
    m_serverAddr.sin_addr.s_addr = INADDR_ANY;
    m_serverAddr.sin_port = htons(port);
    memset(&m_serverAddr.sin_zero, '\0',8);


    if(bind(m_listenerSocket, (struct sockaddr*)&m_serverAddr, sizeof(m_serverAddr))<0)
        err_sys("bind error");

    if(listen(m_listenerSocket, 10)<0)
        err_sys("listen ERROR");
    FD_SET(m_listenerSocket, &m_masterFdSet);
    m_maxFd = m_listenerSocket;
    return true;
}

bool SelectServer::_accept()
{
    sockaddr_in clientAddr;
    uint32_t clientAddrLen = sizeof(clientAddr);
    int newClientFd = accept(m_listenerSocket, (struct sockaddr*)&clientAddr, (socklen_t*)&clientAddrLen);
    printf("accept called , new fd is %d", newClientFd);
    if(newClientFd == -1)
        err_sys("Server Accept Failed ");
    FD_SET(newClientFd, &m_masterFdSet);
    m_maxFd = (newClientFd > m_maxFd)?newClientFd: m_maxFd;
    onNewConnection(clientAddr);
    return true;
}

bool SelectServer::_receive(int clientFd)
{
    uint32_t nbytes = recv(clientFd, buf, sizeof(buf), 0);
    printf("_receive called .. nbytes is %d, buffer is %s", nbytes, buf);
    std::string sData(buf, nbytes);
    onClientData(clientFd, sData);
    if(nbytes > 0)
        broadcast(sData);
    else
    {
        printf("socket %d has something wrong since nbytes == %d", clientFd, nbytes);
        removeClient(clientFd);
    }
    return true;
}

bool SelectServer::removeClient(int clientFd)
{
    onClientBroken(1);
    close(clientFd);
    FD_CLR(clientFd, &m_masterFdSet);
    return true;
}


bool SelectServer::pulse()
{
    m_readFdSet = m_masterFdSet;
    if(select(m_maxFd+1, &m_readFdSet, NULL, NULL, NULL)<0)
        err_sys("SelectServer error ");


    for (int i=0; i <= m_maxFd; i++)
    {
        if(FD_ISSET(i, &m_masterFdSet))
        {
            if(i == m_listenerSocket)
            {
                return _accept();
            }
            else
            {
                return _receive(i);
            }
        }
    }
    return true;
}

void SelectServer::broadcast(const std::string & sData)
{
    for(int i=0;i<m_maxFd;i++)
    {
        if(FD_ISSET(i, &m_masterFdSet )&& (i != m_listenerSocket))
            if(send(i, sData.c_str(), sData.size(), 0 )< 0)
                err_sys("send to error");
    }
}
