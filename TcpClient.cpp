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


#include "TcpClient.h"

char *TcpClient::HostToIp ( char *hostname )
{
    char *retVal;
    retVal = new char[100];
    if ( isdigit ( hostname[0] ) )
        return hostname;
    struct hostent *host;
    if ( ( host = gethostbyname ( hostname ) ) == NULL )
    {
        perror ( "nslookup failed" );
        return "error";
    }
    strcpy ( retVal, inet_ntoa ( * ( ( struct in_addr * ) host->h_addr ) ) );
    return retVal;
}

char *TcpClient::GetHostName()
{
    return _hostname;
}

void TcpClient::SetHostName ( char *host )
{
//   cout << "sizeof(hostname)="<<sizeof(_hostname)<<"   strlen(host)="<<strlen(host)<<endl;
    strcpy ( _hostname, host );
    strcpy ( _ip, HostToIp ( _hostname ) );
}

TcpClient::TcpClient()
{
    _ip = new char[100];
    _hostname = new char[1024];/*
     cout << "sizeof(hostname)="<<sizeof(_hostname)<<endl;
     cout << "sizeof(ip)="<<sizeof(_ip);*/
    _port = 21;
}

char *TcpClient::GetIp()
{
    return _ip;
}

TcpClient::TcpClient ( char *ip )
{
    _ip = new char[100];
    _hostname = new char[1024];
    if ( isdigit ( ip[0] ) )
        _ip = ip;
    else
    {
        strcpy ( _hostname, ip );
        strcpy ( _ip, HostToIp ( _hostname ) );
    }
}



int TcpClient::ConnectToServer()
{
    /* cream socketul */
    if ( ( _Socket = socket ( AF_INET, SOCK_STREAM, 0 ) ) == -1 )
    {
        perror ( "Eroare la socket().\n" );
        return errno;
    }
    if ( !isdigit ( _ip[0] ) )
    {
        strcpy ( _hostname, _ip );
        strcpy ( _ip, HostToIp ( _hostname ) );
    }
    _CmdSocket = socket ( AF_INET, SOCK_STREAM, 0 );
    /* umplem structura folosita pentru realizarea conexiunii cu serverul */
    /* familia socket-ului */
    server.sin_family = AF_INET;
    /* adresa IP a serverului */
    server.sin_addr.s_addr = inet_addr ( _ip );
    /* portul de conectare */
    server.sin_port = htons ( _port );

    /* ne conectam la server */
    if ( connect ( _Socket, ( struct sockaddr * ) &server,sizeof ( sockaddr ) ) == -1 )
    {
        perror ( "[client]Eroare la connect().\n" ); //ErrorHandler
        return errno;
    }
    char buffer[256];
    strcpy ( buffer, ReceiveMessage() );
    if ( GetCode ( buffer ) == 220 )
    {
        cout << "\nConnected to server: " << this->_ip << endl;
        return 0;
    }
    else
    {
        cout << "\nConnection failed";
        return 1;
    }
    return 1;
}

int TcpClient::GetCode ( char *msg )
{
    char tmp[4];
    if ( msg == NULL || strlen ( msg ) <= 3 )
        return 0;
    for ( int i = 0; i < 3; i++ ) tmp[i] = msg[i];
    tmp[3] = '\0';
    int code = atoi ( tmp );
    return code;
}


void TcpClient::SetIp ( char *ip )
{
    _ip = ip;
}

void TcpClient::SetPort ( int port )
{
    _port = port;
}



char *TcpClient::ReceiveMessage()
{
    char *buffer;
    buffer = new char[2048];
    string message;
    bzero ( buffer,2048 );
    char *retMsg;
    int size = 0;
    do
    {
        size = read ( _Socket,buffer,2047 );
        if ( size > 0 )
            message += buffer;
        else
            break;
    }
    while ( buffer[strlen ( buffer ) - 1] != '\n' );

    if ( size > 0 )
    {
        retMsg = new char[message.length() + 1];
        strcpy ( retMsg, message.c_str() );
        int size = strlen ( retMsg );
        for ( int i = size - 1; i > size - 3; i-- )
            if ( retMsg[i] == '\n' || retMsg[i] == '\r' )
                retMsg[i] = '\0';
        return retMsg;
    }
    else if ( size == 0 ) return "";
    else throw ( new Exception ( "Error while receiving message", RECV_EXCEPTION, errno ) ); //ErrorHandler
}

int TcpClient::SendMessage ( char *buffer )
{
    int size = 0;
    char message[strlen ( buffer+5 ) ];
    strcpy ( message, buffer );
    strcat ( message, "\n" );
    size = write ( _Socket, message, strlen ( message ) );
    if ( size < 0 )
    {
        Exception *ex = new Exception();
        throw ( new Exception ( "Error while sending message:", SEND_EXCEPTION, errno ) );
    }
    return size;
}

int TcpClient::CloseConnection()
{
    SendMessage ( "QUIT" );
    close ( _Socket );
    return 0;
}



TcpClient::~TcpClient()
{

}

