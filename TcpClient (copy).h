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
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <string>
using namespace std;

extern int errno;

class TcpClient
{
private:
    int _port;
    int _Socket;// descriptorul de socket
    struct sockaddr_in server;
    string _ip, _hostname;
public:
    TcpClient();
    TcpClient(string hostname);
    int ConnectToServer();
    int CloseConnection();
    int SendMessage(string msg);
    char *ReceiveMessage();
    void MsgLoop();
    //getters
    int GetPort();
    int GetSocket();
    string GetIp();
    string GetHostName();

    //setters
    void SetPort(int port);
    void SetSocket(int socket);
    void SetIp(string ip);
    void SetHostName(string hostname);

    TcpClient(const TcpClient& other);
    virtual ~TcpClient();
    virtual TcpClient& operator=(const TcpClient& other);
    virtual bool operator==(const TcpClient& other) const;
};

#endif // TCPCLIENT_H
