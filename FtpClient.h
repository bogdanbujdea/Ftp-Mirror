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


#ifndef FTPCLIENT_H
#define FTPCLIENT_H
#include "TcpClient.h"
#include <fstream>

using namespace std;
#define 	PORT_ERROR 		11
#define 	CONNECTION_ERROR 	12
#define		SEND_ERROR		13
#define		EXCEPTION_CAUGHT	14
#define		LIST_CMD_FAILED		15
void *WaitForMessage( void *ptr );

class FtpClient : public TcpClient
{
    char *userName, *password;
    char *Result;

public:
    int Refresh;
    FtpClient(char *ftpHost);
    FtpClient();
    int Login(); //login function
    int Login(char *username, char *password); //overloaded login function
    int List(char *dir); //gets a list of files and directories from the directory that it's sent as a parameter to the function
    int ChangeDir(char *dir); //changes the current directory on the server
    int DownloadFile(char *filePath, char *localPath); //makes the proper arrangements for downloading a file
    char *GetPassword();
    char *GetUsername();
    char *GetCurrentDirectory(); //returns current directory
    void SetPassword(char *pass);
    void SetUserName(char *name);
    int GetSocket();
    char* GetResponse();
    int DownloadFolder(char *dir, char *localdir); //downloads a directory
    int GetFile(char *filePath, char *localPath); //this function downloads and saves a file on the disk, the size of the file is returned
    char *GetResult();
    int mkdirs(char *remotePath, char *localPath); //makes multiple directories, for example, mkdir("/a/b/c/d") will create 4 directories
    int GoToParentDirectory(); //goes to parent directory on the server
    virtual ~FtpClient();
};

#endif // FTPCLIENT_H
