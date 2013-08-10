/////客户端程序
#include "stdafx.h"

#include< stdio.h >
#include< stdlib.h >
#include< windows.h >
#include< winsock.h >
#include< string.h > 

#pragma comment( lib, "ws2_32.lib" )

#define PORT 9999
#define BACKLOG 10
#define TRUE 1
#define MAXDATASIZE 100


void socketClient( void )
{

	int iClientSock;
	char buf[ MAXDATASIZE ];
	struct sockaddr_in ServerAddr;
	int numbytes;
	// struct hostent *he;
	WSADATA WSAData;

	if( WSAStartup( MAKEWORD( 1, 1 ), &WSAData ) )//初始化
	{
		printf( "initializationing error!\n" );
		WSACleanup( );
		exit( 0 );
	}

	if( ( iClientSock = socket( AF_INET, SOCK_STREAM, 0 ) ) == INVALID_SOCKET )
	{
		printf( "创建套接字失败!\n" );
		WSACleanup( );
		exit( 0 );
	}

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons( PORT );
	// ServerAddr.sin_addr = *( ( struct in_addr * )he->h_addr );
	ServerAddr.sin_addr.s_addr = inet_addr( "192.168.115.129" );//记得换IP
	memset( &( ServerAddr.sin_zero ), 0, sizeof( ServerAddr.sin_zero ) );

	if( connect( iClientSock, ( struct sockaddr * ) & ServerAddr, sizeof( struct sockaddr ) ) == -1 )
	{
		printf( "connect失败!" );
		WSACleanup( );
		exit( 0 );
	}

	int i=0;
	memcpy(buf, "Hello World", sizeof("Hello World"));
	while(i++<100000)
	{
		send(iClientSock,buf,MAXDATASIZE,0);
		numbytes = recv( iClientSock, buf, MAXDATASIZE, 0 );
		if( numbytes == -1 )
		{
			printf( "recv失败!" );
			WSACleanup( );
			exit( 0 );
		}
		buf[ numbytes ] = '\0';
		//printf( "Received: %s", buf );
	}

	closesocket( iClientSock );
	WSACleanup( );
}
