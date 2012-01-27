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

TcpClient::TcpClient()
{

}

TcpClient::TcpClient(string hostname)
{
  if(isalpha(hostname[0]))
    _ip = hostname;
  else
    _hostname = hostname;
  _port = 21;
    cout << "ip-ul este:" << _ip;
    cout << "\nhostname=" << _hostname << endl;
}

string HostToIp(string hostname)
{
  char *retVal;
  retVal = new char[100];
	if(!isalpha(hostname[0]))
	  return hostname;
	struct hostent *host;
	if((host = gethostbyname(hostname.c_str())) == NULL)
	{
		perror("(mini) nslookup failed");
		exit(1);
	}
	//server.sin_addr.s_addr = *((unsigned long *) host->h_addr_list[0]);
	strcpy(retVal, inet_ntoa(*((struct in_addr *)host->h_addr)));
	string tmp = retVal;
	return tmp;
}

void TcpClient::MsgLoop()
{
  char buffer[256];
  int n;
  while(1) {
 	bzero(buffer,256);
    n = read(_Socket,buffer,255);
    if (n < 0) 
         printf("ERROR reading from socket");
    printf("%s\n",buffer);
    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    n = write(_Socket,buffer,strlen(buffer));
    if (n < 0) 
         printf("ERROR writing to socket");
   
}
}

int TcpClient::ConnectToServer()
{
    /* cream socketul */
    if ((_Socket = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror ("Eroare la socket().\n"); //ErrorHandler
        return errno;
    }

    /* umplem structura folosita pentru realizarea conexiunii cu serverul */
    /* familia socket-ului */
    if(_hostname.length() > 0)
      _ip = _hostname;
    _ip = HostToIp(_ip.c_str());
    
    cout << "ip-ul este:" << _ip;
    cout << "\nhostname=" << _hostname << endl;
    server.sin_family = AF_INET;
    /* adresa IP a serverului */
    server.sin_addr.s_addr = inet_addr(_ip.c_str());
    /* portul de conectare */
    server.sin_port = htons (_port);

    /* ne conectam la server */
    if (connect (_Socket, (struct sockaddr *) &server,sizeof (sockaddr)) == -1)
    {
        perror ("[client]Eroare la connect().\n"); //ErrorHandler
        return errno;
    }
    cout << "Connected to server: " << this->_ip << endl;
    return 1;
}

char *TcpClient::ReceiveMessage()
{
    char *buffer;
    buffer = new char[100];
    bzero(buffer,100);
   if(read(_Socket,buffer,99))
    /*if (n < 0) 
         error("ERROR reading from socket");
    if (read (_Socket, msg, 99) < 0)
    {
      cout<<"eroare in read\n";
        //perror ("[client]Eroare la read() de la server.\n");
        return "error";
    }
    cout<<"\ndupa read";*/
    return buffer;
   else return "error";
}

int TcpClient::SendMessage(string msg)
{
  char *buffer;
  buffer = new char[256];
    if (msg.length() == 0)
    {
        cout << "Message:";
        gets(buffer);
    }
    else strcpy(buffer, msg.c_str());
    if (write (_Socket, buffer, strlen(buffer)) <= 0)
    {
        perror ("[client]Eroare la write() spre server.\n");
        return errno;
    }
    return strlen(buffer);
}

int TcpClient::CloseConnection()
{
    close (_Socket);
    return 1;
}

TcpClient::TcpClient(const TcpClient& other)
{

}

TcpClient::~TcpClient()
{

}

TcpClient& TcpClient::operator=(const TcpClient& other)
{
    return *this;
}

bool TcpClient::operator==(const TcpClient& other) const
{
///TODO: return ...;
}

