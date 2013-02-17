#ifndef TESTSOCKET_H
#define TESTSOCKET_H

#include "apue.h"
#include <unistd.h>
//#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//#include <string>
#include <stdlib.h>

int TestSyncBlock(int nport)
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        err_sys("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = nport;
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              err_sys("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd,
                 (struct sockaddr *) &cli_addr,
                 &clilen);
     if (newsockfd < 0)
          err_sys("ERROR on accept");
     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0)
         err_sys("ERROR reading from socket");
     printf("Server:Here is client message: %s\n",buffer);
     n = write(newsockfd,"message from server",19);
     if (n < 0)
         err_sys("ERROR writing to socket");
     close(newsockfd);
     close(sockfd);
     return 0;
}

int TestSyncBlockClient(int nport)
{
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<0)
        err_sys("ERROR opening socket");
    struct sockaddr_in serv_addr;
    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(nport);
    if((connect(sockfd, (struct sockaddr*)& serv_addr, sizeof(serv_addr)))<0)
        err_sys("error connect socket");
    while(1)
    {
        char* pMsg = "message from client";
        int n = write(sockfd, pMsg, strlen(pMsg));
        if(n < 0)
            err_sys("write socket error");
        char buf[256];
        bzero(buf,256);
        n = read(sockfd, buf, 255);
        if(n<0)
            err_sys("read socket error");
        printf("Msg Size:%d\n",n);
        printf("Client:Here is server message: %s\n", buf);
    }
    close(sockfd);
    return 0;
}


int TestUDP(int nport)
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0 );
    if(sockfd < 0)
        err_sys("open socket error");
    struct sockaddr_in serv_addr;
    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(nport);
    if(bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        err_sys("bind error");

    struct sockaddr_in cli_addr;
    socklen_t clilen =sizeof(cli_addr);
    static int icount = 0;
    char buf[MAXLINE];
    for(;;)
    {
        bzero(buf, MAXLINE);
        int n = recvfrom(sockfd, buf,MAXLINE,0, (struct sockaddr*)&cli_addr, &clilen);
        printf("received:%d",icount);
        if(n>0)
            printf("I got the message,%s\n",buf);
        else
            err_msg("recvfrom errror\n");
    }
}

int TestUDPClient(int nport)
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock<0)
        err_sys("open sock error");
    struct sockaddr_in serv_addr;
    socklen_t servlen = sizeof(serv_addr);
    bzero((char*)&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(nport);
    for(int i = 0 ; i<10000 ; i++)
    {
        char buffer[128] =  {'\0'};
        bzero(buffer, 0);
        sprintf(buffer, "hello world %d", i);
        sendto (sock, buffer, strlen(buffer), 0, (struct sockaddr*)&serv_addr, servlen);
    }
}

#endif
