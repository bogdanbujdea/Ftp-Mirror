/*
    <This software is a FTP client used to download a copy of a FTP server.>
    Copyright (C) 2012  <Bujdea Bogdan> <costinel.bujdea@info.uaic.ro>

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
        strcpy ( _ip, HostToIp ( _hostname ) ); //HostToIp will return the IP if the string ip is a host name, or just the ip otherwise
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
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr ( _ip );
    server.sin_port = htons ( _port );

    if ( connect ( _Socket, ( struct sockaddr * ) &server,sizeof ( sockaddr ) ) == -1 )
    {
        perror ( "[client]Eroare la connect().\n" );
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
    return code; //returns a three digit code
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
    int multiline = 0;
    do
    {
        size = read ( _Socket,buffer,2047 );
        if ( size > 0 )
            message += buffer; //we add the message in a string
        else
            break;
	if(!multiline && buffer[3] == '-') //if we find a '-'
	{
	  multiline = 1; //set the multiline variable to 1
	  bzero(buffer, 2048); //empty the buffer
	  continue; //continue, or we will exit on the next if statement
	}
	if(multiline == 1 && buffer[3] == ' ')
	  break;
	if(buffer[strlen ( buffer ) - 1] == '\n')
	  break;
    }
    while ( 1);
    /*Sometimes, servers might reply with a multi-line message, and if we only want one line
     * the rest of the command is waiting to be received, so next time we want to receive something
     * we won't get what we would expect
     * Example: The FTP server ftp.kde.org sends a multiline message after you list the /pub/kde folder
     * Multiline messages will start with a three digit code followed by a '-' character
     * A normal message, has three digits, followed by a space and a message, so this is how we know
     * what type of message we are receiving
     * ftp.kde.org sends something like this
     * 250-         Hello! This is the ftp server....
     * 250-Some information
     * 250-about the server
     * 250-some rules
     * 250-and the end of message*/
    

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
    strcat ( message, "\n" ); //append a new line to the message, so the server know's we sent the entire message
    int cmdSize = 0; //command size, this will help us ensure that the entire message will be sent
    do
    {
      size = write ( _Socket, message, strlen ( message ) );
      cmdSize += size;
    } while(cmdSize < strlen(buffer));
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

