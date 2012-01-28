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


#include "FtpClient.h"



FtpClient::FtpClient() : TcpClient()
{
    userName = new char[1024];
    password = new char[1024];
    strcpy(userName, "anonymous");
    strcpy(password, "pass@mail.com");

}

FtpClient::FtpClient(char *ip) : TcpClient(ip)
{
    userName = new char[1024];
    password = new char[1024];
    strcpy(userName, "anonymous");
    strcpy(password, "pass@mail.com");

    _port = 21;
}

int FtpClient::Login(char* username, char* password)
{
    strcpy(this->userName, username);
    strcpy(this->password, password);
    return Login();
}

int GetPortFromCode(char *code)
{
    string tmp = code;
    //(138,292,303,223,123,432). just an example of what PASV returns
    int comma = 0, i, k = 0, j;
    for (i = strlen(code) - 1; i > 0; i--)
    {
        if (code[i] == ',')
        {
            comma++;
            if (comma == 2)
                break;
        }
    }
    //,123,432). this is what remains code after j
    char first[10], second[10];
    for (j = i + 1; j < strlen(code); j++)
    {
        if (isdigit(code[j]))
            first[k++] = code[j];
        else break;
    }
    first[k] = '\0';
    k = 0;
    for (i = j + 1; i < strlen(code); i++)
    {
        if (isdigit(code[i]))
            second[k++] = code[i];
        else break;
    }
    second[k] = '\0';
    int port1 = 0, port2 = 0;
    port1 = atoi(first);
    port2 = atoi(second);
    return (port1 * 256) + port2;
}

int FtpClient::List(char *dir)
{
    if (_CmdSocket == -1)
        return -1;
    try
    {
        char buffer[2048];
	pthread_cancel(msgThread);
	
        SendMessage("PASV");
        strcpy(buffer, ReceiveMessage());
	cout << buffer << endl;
        int port = GetPortFromCode(buffer);
        server.sin_port = htons (port);
        /* ne conectam la server */
	_CmdSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (connect (_CmdSocket, (struct sockaddr *) &server,sizeof (sockaddr)) == -1)
        {
            cout << "[CmdClient]Eroare la connect().\n" << endl;
            return errno;
        }
        int tmp = _Socket;
	char tmpDir[2048];
	if(strlen(dir) && strcmp(dir, "this"))
	{
	  strcpy(tmpDir, "LIST ");
	  strcat(tmpDir, dir);
	}
	else strcpy(tmpDir, "LIST");
        SendMessage(tmpDir);
        _Socket = _CmdSocket;
        strcpy(buffer, ReceiveMessage());
	cout << "LIST RESULT:\n" << buffer;
        _Socket = tmp;
	close(_CmdSocket);
    }
    catch (Exception ex)
    {
        cout << ex.Message << endl;
        return ex.ErrorCode;
    }
    return 0;
}

int FtpClient::ChangeDir(char* dir)
{
   try
   {
     char buffer[2048];
     strcpy(buffer, "CWD ");
     strcat(buffer, dir);
    pthread_cancel(msgThread);
     SendMessage(buffer);
     strcpy(buffer, ReceiveMessage());
     cout << buffer << endl;
     int code = GetCode(buffer);
     if (code != 250)
       return code;
     else
       return 0;
   }
   catch(Exception ex)
   {
     cout << ex.Message;
     return ex.ErrorCode;
   }
}


int FtpClient::Login()
{
    try
    {
        char buffer[256];
        int code;
        strcpy(buffer, "USER ");
        strcat(buffer, this->userName);
        SendMessage(buffer); //sending User
        strcpy(buffer, ReceiveMessage()); //receiving confirmation
        code = GetCode(buffer);
        if (code != 331) //if the FTP server sent a code that it's not 331, than we've got problems
            return code;
        strcpy(buffer, "PASS ");
        strcat(buffer, password);
        SendMessage(buffer); //now we have to send the password
        strcpy(buffer, ReceiveMessage());
        code = GetCode(buffer);
        if (code != 230) //if something went wrong, then we send the code back to the calling function
            return code;
    }
    catch (Exception ex)
    {
        cout << ex.Message;
    }
    return 0; //this means that we are logged in
}

FtpClient::FtpClient(const FtpClient& other)
{

}

FtpClient::~FtpClient()
{

}

FtpClient& FtpClient::operator=(const FtpClient& other)
{
    return *this;
}

bool FtpClient::operator==(const FtpClient& other) const
{
///TODO: return ...;
}

