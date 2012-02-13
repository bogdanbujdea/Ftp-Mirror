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


#include "FtpClient.h"
int Sock;
char* GetMessage()
{
    char *buffer;
    buffer = new char[2048];
    string message;
    bzero ( buffer,2048 );
    char *retMsg;
    int size = 0;
    //vezi cum primesti mesajele corect
    //si faci alta fct pt dld fisiere
    do
    {
        size = read ( Sock,buffer,2047 );
        if ( size > 0 )
            message += buffer;
        else
            break;
    }
    while ( message[message.length() - 1] != '\n' );

    if ( message.length() > 0 )
    {
        retMsg = new char[message.length() + 1];
        strcpy ( retMsg, message.c_str() );
        return retMsg;
    }
    else throw ( new Exception ( "Error while receiving message", RECV_EXCEPTION, errno ) ); //ErrorHandler
}


FtpClient::FtpClient() : TcpClient()
{
    userName = new char[1024];
    password = new char[1024];
    strcpy ( userName, "anonymous" );
    strcpy ( password, "pass@mail.com" );
    Result = new char[4096];
}

char *FtpClient::GetUsername()
{
    return userName;
}

char *FtpClient::GetPassword()
{
    return password;
}



FtpClient::FtpClient ( char *ip ) : TcpClient ( ip )
{
    Result = new char[4096];
    userName = new char[1024];
    password = new char[1024];
    strcpy ( userName, "anonymous" );
    strcpy ( password, "pass@mail.com" );
    Refresh = 0;
    _port = 21;
}

int FtpClient::Login ( char* username, char* password )
{
    Result = new char[4096];
    strcpy ( this->userName, username );
    strcpy ( this->password, password );
    return Login();
}

void FtpClient::SetUserName ( char *name )
{
    strcpy ( userName, name );
}

void FtpClient::SetPassword ( char *pass )
{
    strcpy ( password, pass );
}

char* FtpClient::GetResult()
{
    return Result;
}

int GetPortFromCode ( char *code )
{
    string tmp = code;
    //(138,292,303,223,123,432). just an example of what PASV returns
    int comma = 0, i, k = 0, j;
    for ( i = strlen ( code ) - 1; i > 0; i-- )
    {
        if ( code[i] == ',' )
        {
            comma++;
            if ( comma == 2 )
                break;
        }
    }
    //,123,432). this is what remains code after j
    char first[10], second[10];
    for ( j = i + 1; j < strlen ( code ); j++ )
    {
        if ( isdigit ( code[j] ) )
            first[k++] = code[j];
        else break;
    }
    first[k] = '\0';
    k = 0;
    for ( i = j + 1; i < strlen ( code ); i++ )
    {
        if ( isdigit ( code[i] ) )
            second[k++] = code[i];
        else break;
    }
    second[k] = '\0';
    int port1 = 0, port2 = 0;
    port1 = atoi ( first );
    port2 = atoi ( second );
    return ( port1 * 256 ) + port2;
}

char* GetFileName ( char *filePath )
{
    char fileName[2048];
    bzero ( fileName, 2048 );
    int i = 0, k;
    char *token;
    char seps[]   = " ";
    token = strtok ( filePath,seps );
    string vector[10];
    while ( token != NULL )
    {
        vector[i] = token;
        token = strtok ( NULL, seps );
        i++; //number of properties
    }
    strcpy ( fileName, vector[i-1].c_str() );
    return fileName;
}

int FtpClient::DownloadFile ( char *filePath, char *localPath )
{
  int tmpSock = _Socket;
    try
    {
        int code = 0;
        string list[1000];
        int i = 0, j = 0, n;
        char buffer[4096];
        SendMessage ( "TYPE I" );
        strcpy ( buffer, ReceiveMessage() );
        cout << endl << buffer << endl;
        code = GetCode ( buffer );
        if ( code != 200 )
            return code;
        SendMessage ( "PASV" );
        strcpy ( buffer, ReceiveMessage() );
        code = GetCode ( buffer );
        if ( code != 227 )
            return code;
        int port = 0;
        port = GetPortFromCode ( buffer );
        if ( port == 0 )
        {
            return PORT_ERROR;
        }
        server.sin_port = htons ( port );
        /* ne conectam la server */
        _CmdSocket = socket ( AF_INET, SOCK_STREAM, 0 );
        if ( connect ( _CmdSocket, ( struct sockaddr * ) &server,sizeof ( sockaddr ) ) == -1 )
        {
            cout << "[CmdClient]Eroare la connect().\n" << endl;
            perror ( "eroare=" );
            return CONNECTION_ERROR;
        }
        
        char msg[2048];
        strcpy ( msg, "RETR " );
        strcat ( msg, filePath );
        char fileName[2048];
        bzero ( fileName, 2048 );
        strcpy ( fileName, GetFileName ( filePath ) );
        if ( !SendMessage ( msg ) ) //sending RETR command
        {
            return SEND_ERROR;
        }
       
	 _Socket = _CmdSocket;
        int fileSize = GetFile ( fileName, localPath );
        if ( fileSize > 0 )
            cout << "File downloaded with a size of " << fileSize << endl;
	else cout << "File wasn't downloaded\n";
        _Socket = tmpSock;
        strcpy ( buffer, ReceiveMessage() );
        cout << endl << buffer << endl;
        code = GetCode ( buffer );
        if ( code != 150 )
            return code;
        code = -1;
        for ( int i = 0; i < strlen ( buffer ); i++ ) //get code from the rest of the message
        {
            if ( buffer[i] == '\n' && i < strlen ( buffer ) - 3 )
            {
                char tmp[4];
                tmp[0] = buffer[i+1];
                tmp[1] = buffer[i+2];
                tmp[2] = buffer[i+3];
                tmp[3] = '\0';
                code = atoi ( tmp );
            }
        }
        if ( code == -1 )
        {
            strcpy ( buffer, ReceiveMessage() );
            cout << endl << buffer << endl;
            code = GetCode ( buffer );
        }
        if ( code != 226 )
            return code;
        close ( _CmdSocket );
        return 0;
    }
    catch ( Exception ex )
    {
        cout << ex.Message << "\b" << ex.ErrorCode << endl;
	_Socket = tmpSock;
        return EXCEPTION_CAUGHT;
    }
}



int FtpClient::DownloadFolder ( char *dir, char *lDir )
{
    try
    {
        char localdir[4096];
        strcpy ( localdir, lDir );
        int error = ChangeDir ( dir );
        if ( error )
            cout << "Failed to change remote dir to "<<dir<<" , error="<<error<<endl;
        else cout << "\nDirectory was changed to "<<dir<<endl;
        strcat ( localdir, dir );
        if ( !mkdir ( localdir, 0777 )  || errno == EEXIST)
            cout << "\nLocal directory " << localdir << "created"<<endl;
        else
        {
            cout <<"\nLocal directory " << localdir << "wasn't created, err="<<errno<<endl;

        }
        string list[1000];
        int i = 0, j = 0, n = 0;
        if ( !List ( "this" ) )
        {
            while ( j < strlen ( Result ) )
            {
                //list[i] = new string;
                while ( Result[j] == '\r' || Result[j] == '\n' )
                    j++;
                while ( Result[j] != '\r' && Result[j] != '\n' && Result[j] != '\0' )
                {
                    list[i] += Result[j++];
                }
                i++;
                j++;
            }
            n = i;
        }
        else
        {
            return LIST_CMD_FAILED;
        }
        for ( i = 0; i < n; i++ )
        {
            int found;
            if ( list[i][0] == 'l' )
                continue;
            if ( list[i][0] == 'd' )
            { //folder sau fisier cu spatii
                
                if ( list[i][list[i].length()-1] != '.')
                {
		  found = list[i].find_last_of (' ');
                string folder = list[i].substr ( found+1 );
                    char fd[256];
                    strcpy ( fd, dir );
                    if ( dir[strlen ( dir ) - 1] != '/' )
                        strcat ( fd, "/" );
                    strcat ( fd, folder.c_str() );
                    DownloadFolder ( fd, lDir );
                }
            }
            else if(list[i].length() > 0)
            {
                char seps[]   = " ";
                char *token;
                char file[512];
                strcpy ( file, list[i].c_str() );
                token = strtok ( file, seps ); //ia cate un cuvant
                string vector[20];
                int prop = 0;
                while ( token != NULL )
                {
                    vector[prop] = token; //punem cuvantul in vector
                    token = strtok ( NULL, seps );
                    prop++; //numarul de proprietati
                }
                bzero ( file, 512 );
                if ( prop > 9 )
                {
                    for ( int x = 8; x < prop; x++ )
                    {
                        strcat ( file, vector[x].c_str() );
                        strcat ( file, " " );
                    }
                    file[strlen ( file ) - 1] = '\0';
                }
                else	strcpy ( file, vector[prop-1].c_str() );
		if(prop < 9)
		  continue;
                char localPath[4096];
                strcpy ( localPath, localdir );
                if ( localdir[strlen ( localdir )-1] != '/' )
                    strcat ( localPath, "/" );
                strcat ( localPath, file );
		if(i > 57)
		  i = i;
                cout<<"\nDownloading file:"<<file;
                int error = 0;
                error = DownloadFile ( file, localPath );
                if ( error == 0 )
                    cout << "\nFile was downloaded succesfully\n";
                else cout << "\nWe encountered an error while downloading the file, error="<<error<<"\n";
            }
        }

    }
    catch ( Exception ex )
    {
        cout << ex.Message << endl << ex.ErrorCode << endl;
        return 1;
    }
    GoToParentDirectory();
    return 0;
}

int FtpClient::GoToParentDirectory()
{
    try
    {
        if (!SendMessage("CDUP")) //go to parent directory
            return 1;
        char buffer[512];
        strcpy(buffer, ReceiveMessage());
        chdir("..");
        //getcwd(buffer, 512);
        //strcpy(localPath, buffer);
        int code = GetCode(buffer);
        //handle the code here
	return 0;
    }
    catch (Exception ex)
    {
        cout << ex.Message << endl;
        return 1;
    }
    return 0;
}

int FtpClient::GetFile ( char *filePath, char *localPath )
{
    char *buffer;
    buffer = new char[2048];
    bzero ( buffer,2048 );
    int size = 0, fileSize = 0;
    if(Refresh)
    {
      ifstream f(localPath, ios::in );
      if(f.is_open())
	return 0;
      f.close();
    }
    ofstream file ( localPath, ios::out | ios::binary );
    if ( !file.is_open() && errno != 2)
        throw ( new Exception ( "Couldn't create new file", FILE_CREATE_EXCEPTION, errno ) );
    do
    {
        size = read ( _Socket,buffer,2047 );
        if ( size > 0 )
        {
            fileSize += size;
            file.write ( buffer, size );
        }
        else
            break;
    }
    while ( size > 0 );
    file.close();
    if ( fileSize == 0 )
        throw ( new Exception ( "Error while receiving file", RECV_EXCEPTION, errno ) ); //ErrorHandler
    else return fileSize;
}

int FtpClient::GetSocket()
{
    return _Socket;
}

int FtpClient::List ( char *dir )
{
    if ( _CmdSocket == -1 )
        return -1;
    try
    {
        int code = 0;
        char buffer[4096];
        SendMessage ( "PASV" );
        strcpy ( buffer, ReceiveMessage() );
        code = GetCode ( buffer );
        if ( code != 227 )
            return code;
        cout << endl << buffer << endl;
        int port = GetPortFromCode ( buffer );
        server.sin_port = htons ( port );
        /* ne conectam la server */
        _CmdSocket = socket ( AF_INET, SOCK_STREAM, 0 );
        if ( connect ( 	_CmdSocket, ( struct sockaddr * ) &server,sizeof ( sockaddr ) ) == -1 )
        {
            perror ( "[CmdClient]Eroare la connect().\n" );
            return errno;
        }
        else cout << "CmdSocket conectat\n";
        int tmp = _Socket;
        char tmpDir[4096];
        if ( strlen ( dir ) && strcmp ( dir, "this" ) )
        {
            strcpy ( tmpDir, "LIST " );
            strcat ( tmpDir, dir );
        }
        else strcpy ( tmpDir, "LIST" );
        SendMessage ( tmpDir );
        _Socket = _CmdSocket;
        usleep(100);
        strcpy ( buffer, GetResponse() );
        usleep(100);
        _Socket = tmp;
        cout << endl << "\Listing directory \n" << buffer<<endl;
        if ( strlen ( buffer ) == 0 )
            return LIST_CMD_FAILED;
        bzero ( Result, 4096 );
        strcpy ( Result, buffer );

        strcpy ( buffer, ReceiveMessage() );
        cout << endl << buffer << endl;
        code = GetCode ( buffer );
        if ( code != 150 )
            return code;
        code = -1;
        for ( int i = 0; i < strlen ( buffer ); i++ )
        {
            if ( buffer[i] == '\n' && i < strlen ( buffer ) - 3 )
            {
                char tmp[4];
                tmp[0] = buffer[i+1];
                tmp[1] = buffer[i+2];
                tmp[2] = buffer[i+3];
                tmp[3] = '\0';
                code = atoi ( tmp );
                break;
            }
        }
        if ( code == -1 )
        {
            strcpy ( buffer, ReceiveMessage() );
            cout << endl << buffer << endl;
            code = GetCode ( buffer );
        }
        if ( code != 226 )
            return code;
        close ( _CmdSocket );
    }
    catch ( Exception ex )
    {
        cout << ex.Message << endl;
        return ex.ErrorCode;
    }
    return 0;
}





int FtpClient::mkdirs(char *remotePath, char *localPath)
{
    char *token = strtok(remotePath,  "//");
    char newDir[1024];
    bzero(newDir, 1024);
    strcpy(newDir, localPath);
    while(token != NULL)
    {
      strcat(newDir, "/");
      strcat(newDir, token);
      mkdir ( newDir, 0777 );
      token = strtok(NULL, "/");
    }
    return 0;
}

char *FtpClient::GetCurrentDirectory()
{
    try
    {
        char buffer[2048];
        bzero ( buffer, 2048 );
        SendMessage ( "PWD" );
        strcpy ( buffer, ReceiveMessage() );
        int code = GetCode ( buffer );
        if ( code == 257 )
        {
            int i = 0;
            for ( i = 0; i < strlen ( buffer ) && buffer[i] != '\"'; i++ );
            char dir[2048];
            bzero ( dir, 2048 );
            strcpy ( dir, buffer + ( i + 1 ) );
            for ( i = 0; i < strlen ( dir ) && dir[i] != '\"'; i++ );
            dir[i] = '\0';
            return dir;
        }
        else
            return "error";
    }
    catch ( Exception ex )
    {
        cout << ex.Message;
        return ex.Message;
    }
}

int FtpClient::ChangeDir ( char* dir )
{
    try
    {
        char buffer[2048];
        strcpy ( buffer, "CWD " );
        strcat ( buffer, dir );
        SendMessage ( buffer );
        strcpy ( buffer, ReceiveMessage() );
        cout << "CWD "<<dir<< " "<<buffer << endl;
        int code = GetCode ( buffer );
        if ( code != 250 )
            return code;
        else
            return 0;
    }
    catch ( Exception ex )
    {
        cout << ex.Message;
        return ex.ErrorCode;
    }
}

char *FtpClient::GetResponse()
{
    //This function receives a response from the server
    //in this format ABC <Message><CRLF>, where ABC is
    //a three digit code, Message is a string, and <CRLF>
    //represents end of message
    char buffer[2048];
    string message;
    bzero ( buffer,2048 );
    char *retMsg;
    int size = 0;
    size = read ( _Socket,buffer,4 );
    message = buffer;
    int Multiline;
    if (buffer[3] == '-') //this is a multiline message
        Multiline = 1;
    do
    {
        size = read(_Socket, buffer, 2047);
        if ( size > 0 )
            message += buffer;
        if (!Multiline && buffer[strlen ( buffer ) - 1] != '\n' && buffer[strlen(buffer)-2] == '\r')
            break;
        else if (Multiline && buffer[3]=='-')
            break;
	if(size == 0)
	  break;
    }
    while (1);

    if ( message.length() > 0 )
    {
        retMsg = new char[message.length() + 1];
        strcpy ( retMsg, message.c_str() );
        int size = strlen ( retMsg );
        return retMsg;
    }
    else if ( message.length() == 0 ) return "123 Nothing Received"; //return some error
    else throw ( new Exception ( "Error while receiving message", RECV_EXCEPTION, errno ) ); //ErrorHandler
}


int FtpClient::Login()
{
    try
    {
        char buffer[2048];
        int code;
        strcpy ( buffer, "USER " );
        strcat ( buffer, this->userName );
	cout << buffer << endl;
        if (!SendMessage ( buffer ))
        {
            perror("Can't send message\n");
            return 1;
        }
        strcpy ( buffer, ReceiveMessage() ); //receiving confirmation
	cout << buffer << endl;
        code = GetCode ( buffer );
        if ( code != 331 ) //if the FTP server sent a code that it's not 331, than we've got problems
            return code;
        strcpy ( buffer, "PASS " );
        strcat ( buffer, password );
	cout << buffer << endl;
        SendMessage ( buffer ); //now we have to send the password
        strcpy ( buffer, ReceiveMessage() );
	cout << buffer << endl;
        code = GetCode ( buffer );
        if ( code != 230 ) //if something went wrong, then we send the code back to the calling function
            return code;
    }
    catch ( Exception ex )
    {
        cout << ex.Message;
    }
    return 0; //this means that we are logged in
}

FtpClient::~FtpClient()
{

}



