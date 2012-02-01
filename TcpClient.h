/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2012  <copyright holder> <email>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#ifndef TCPCLIENT_H
#define TCPCLIENT_H
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <string>
//#include <pthread.h>
#include "Exception.h"
using namespace std;
extern int errno;

class TcpClient
{
protected:
    int _port;
    int _Socket, _CmdSocket;// descriptorul de socket
    struct sockaddr_in server;
    char *_ip, *_hostname;
    
public:
  
    TcpClient();
    TcpClient(char* hostname);
    
    char *HostToIp(char *hostname);
    
    int ConnectToServer();
    int CloseConnection();
    int SendMessage(char *buffer);
    char *ReceiveMessage();
    int GetCode(char *msg);

    //getters
    int GetPort();
    int GetSocket();
    char* GetIp();
    char* GetHostName();

    //setters
    void SetPort(int port);
    void SetSocket(int socket);
    void SetIp(char *ip);
    void SetHostName(char* hostname);
    
    
    pthread_t msgThread;
    virtual ~TcpClient();
};

#endif // TCPCLIENT_H
