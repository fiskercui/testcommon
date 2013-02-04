#ifndef SELECT_H
#define SELECT_H

#include "apue.h"
#include <vector>
#include <string>
#include <sys/types.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
//typedef int uint32_t;

class SelectServer
{
public:
    SelectServer();
    bool init();
    bool _listen(uint32_t port);
    bool pulse();
    void broadcast(const std::string& sData);

protected:
    virtual void onNewConnection(const sockaddr_in& clientAddr){

    }
    virtual void onClientData(int clientfd, const std::string& sData){

    }
    virtual void onClientBroken(int errCode){

    }
private:
    bool _accept();
    bool _receive(int cliFd);
    bool removeClient(int cliFd);

    fd_set m_masterFdSet;// master file desc filelist//
    fd_set m_readFdSet;//tmp file desc list for select//
    sockaddr_in m_serverAddr;// server address//
    int m_maxFd;// maxium file desc number//
    int m_listenerSocket;// listening sock desc//
    char buf[1024];// buffer for clientdata//
};

#endif
