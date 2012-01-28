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
int Sock;
char* GetMessage()
{
    char *buffer;
    buffer = new char[2048];
    string message;
    bzero(buffer,2048);
    char *retMsg;
    int size = 0;
    //vezi cum primesti mesajele corect
    //si faci alta fct pt dld fisiere
    do {
        size = read(Sock,buffer,2047);
        if (size > 0)
            message += buffer;
        else
            break;
    }
    while (message[message.length() - 1] != '\n');

    if (message.length() > 0)
    {
        retMsg = new char[message.length() + 1];
        strcpy(retMsg, message.c_str());
        return retMsg;
    }
    else throw(new Exception("Error while receiving message", RECV_EXCEPTION, errno)); //ErrorHandler
}

void *WaitForMessage( void *ptr )
{
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
    while (1)
    {
        cout << endl << "Received in thread: ";
	cout << GetMessage() << endl;
    }
}

FtpClient::FtpClient() : TcpClient()
{
    userName = new char[1024];
    password = new char[1024];
    strcpy(userName, "anonymous");
    strcpy(password, "pass@mail.com");
    Result = new char[4096];
}

FtpClient::FtpClient(char *ip) : TcpClient(ip)
{
    Result = new char[4096];
    userName = new char[1024];
    password = new char[1024];
    strcpy(userName, "anonymous");
    strcpy(password, "pass@mail.com");

    _port = 21;
}

int FtpClient::Login(char* username, char* password)
{
    Result = new char[4096];
    strcpy(this->userName, username);
    strcpy(this->password, password);
    return Login();
}

void FtpClient::SetUserName(char *name)
{
    strcpy(userName, name);
}

void FtpClient::SetPassword(char *pass)
{
    strcpy(password, pass);
}

char* FtpClient::GetResult()
{
    return Result;
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

char* GetFileName(char *filePath)
{
    char fileName[2048];
    bzero(fileName, 2048);
    int i = 0, k;
    for (k = strlen(filePath) - 1; k >= 0; k--)
        if (filePath[k] != '/' && filePath[k] != ' ')
            fileName[i++] = filePath[k];
        else break;
    fileName[i] = '\0';

    int len;
    len=strlen(fileName);

    for (int i=0,j=len-1;i<len/2;i++,j--)
    {
        swap(fileName[i], fileName[j]);
    }

    fileName[len]='\0';
    return fileName;
}

int FtpClient::DownloadFile(char *filePath, char *localPath)
{
    try
    {
        string list[1000];
        int i = 0, j = 0, n;
        List(filePath);
        ReceiveMessage();
        ReceiveMessage();
        //folosesti list pentru fisier
        //daca e fisier(nume[0] != 'd'), si ai drept de citire, at schimbi pe ASCII sau Binary si il dld
        //vezi cum faci sa il dld, in ce mod, poate e in tipul fisierului
        //si mai vezi cum iei toate atributele, poate faci si o clasa/structura pentru asta, sau poate este deja
        //acum fugi si dormi :>
        //dupa ce determini toate chestiile de mai sus, apelezi RETR, si il dld
        //dar vezi cum faci pe bucati...ca poate fi prea mare
        while (j < strlen(Result))
        {
            //list[i] = new string;
            while (Result[j] != '\n')
            {
                list[i] += Result[j++];
            }
            i++;
            j++;
        }
        n = i;
        for (i = 0; i < n; i++)
        {
            cout <<"\nlist["<<i<<"]="<<list[i];
        }
        char buffer[4096];
        if (list[0][0] != 'd')
        {
            cout << "file is not directory\n";
            sleep(3);
            pthread_cancel(msgThread);
            SendMessage("TYPE I");
            strcpy(buffer, ReceiveMessage());
            cout << endl << buffer << endl;
            SendMessage("PASV");
            strcpy(buffer, ReceiveMessage());
            int port = GetPortFromCode(buffer);
            server.sin_port = htons (port);
            /* ne conectam la server */
            _CmdSocket = socket(AF_INET, SOCK_STREAM, 0);
            if (connect (_CmdSocket, (struct sockaddr *) &server,sizeof (sockaddr)) == -1)
            {
                cout << "[CmdClient]Eroare la connect().\n" << endl;
                return errno;
            }
            int tmpSock = _Socket;
            char msg[2048];
            strcpy(msg, "RETR ");
            strcat(msg, filePath);
            char fileName[2048];
            bzero(fileName, 2048);
//             int i = 0, k;
//             for (k = strlen(filePath) - 1; k >= 0; k--)
//                 if (filePath[k] != '/' && filePath[k] != ' ')
//                     fileName[i++] = filePath[k];
//                 else break;
//             fileName[i] = '\0';
//             cout << "reversed file name="<<fileName << endl;
//             int len;
//             len=strlen(fileName);
//
//             for (int i=0,j=len-1;i<len/2;i++,j--)
//             {
//                 swap(fileName[i], fileName[j]);
//             }
//
//             fileName[len]='\0';
            strcpy(fileName, GetFileName(filePath));
            cout << "\nfileName=" << fileName << endl;
            SendMessage(msg); //sending RETR command
            _Socket = _CmdSocket;
            int fileSize = GetFile(fileName, localPath);
            if (fileSize)
                cout << "File downloaded with a size of " << fileSize << endl;
            else cout << "File wasn't downloaded\n";
            _Socket = tmpSock;
            close(_CmdSocket);
	    return 0;
        }
        //strcpy(buffer, Result);
    }
    catch (Exception ex)
    {
        cout << ex.Message << "\b" << ex.ErrorCode << endl;
	return 1;
    }
}

int FtpClient::DownloadFolder(char *dir, char *lDir)
{
    try
    {


        char localdir[4096];
        strcpy(localdir, lDir);
        int error = ChangeDir(dir);
        if (error)
            cout << "failed to change dir="<<dir<<" , errno="<<error<<endl;
        else cout << "\nRemote directory was changed to "<<dir<<endl;
        strcat(localdir, dir);
        if (!mkdir(localdir, 0777))
            cout << "\nDirectory " << localdir << "created"<<endl;
        else
        {
            cout <<"\nDirectory " << localdir << "wasn't created, err="<<errno<<endl;
            perror("\nerror=");

        }
        string list[1000];
        int i = 0, j = 0, n = 0;
	int ret = CreateThread();
	cout << "thread="<<ret<<endl;
	sleep(2);
	pthread_cancel(msgThread);
        if (!List(dir))
        {
            cout << endl << "message1=" << ReceiveMessage();
            usleep(100);
            cout << endl << "message2=" << ReceiveMessage();
            usleep(100);
            while (j < strlen(Result))
            {
                //list[i] = new string;
                while (Result[j] == '\r' || Result[j] == '\n')
                    j++;
                while (Result[j] != '\r' && Result[j] != '\n' && Result[j] != '\0')
                {
                    list[i] += Result[j++];
                }
                int found;
		
                if (list[i][0] == 'd')
                {
		  found = list[i].find_last_of(' ');
                    string folder = list[i].substr(found+1);
                    char fd[256];
                    strcpy( fd, dir);
                    if (dir[strlen(dir) - 1] != '/')
                        strcat(fd, "/");
                    strcat(fd, folder.c_str());
                    DownloadFolder(fd, lDir);
                }
                else
                {
		  found = list[i].find_last_of(' ');
		  if(found == -1)
		    break;
                    char file[256];
                    strcpy(file, list[i].substr(found+1).c_str());
                    cout << "file ="<< file << endl;
		    char localPath[4096];
		    strcpy(localPath, localdir);
		    if(localdir[strlen(localdir)-1] != '/')
		      strcat(localPath, "/");
		    strcat(localPath, file);
		    cout<<"\nFile To Download="<<localPath<<endl;
                    if(!DownloadFile(file, localPath))
		      cout << "\nFile was downloaded succesfully\n";
		    else cout << "\nWe encountered an error while downloading the file\n";
                }
                i++;
                j++;
            }
            n = i;
        }
//         for (i = 0; i < n; i++)
//         {
//             cout <<"\nlist["<<i<<"]="<<list[i];
//         }
//

    }
    catch (Exception ex)
    {
        cout << ex.Message << endl << ex.ErrorCode << endl;
	return 1;
    }
    //first time, create root on local
    //if(dir == '/') create root, else create directory
    //SetCurrentDir(dir) -on local
    //get a list of files from this dir on server with List(dir)
    //iterate the list of files
    //for each dir, call this function
    //for each file, save them on local directory
    //
    return 0;
}

int FtpClient::GetFile(char *filePath, char *localPath)
{
    char *buffer;
    buffer = new char[2048];
    bzero(buffer,2048);
    int size = 0, fileSize = 0;
    ofstream file(localPath, ios::out | ios::binary);
    if (!file.is_open())
        throw (new Exception("Couldn't create new file", UNKNOWN_EXCEPTION, errno));
    do {
        size = read(_Socket,buffer,2047);
        if (size > 0)
        {
            fileSize += size;
            file.write(buffer, size);
        }
        else
            break;
    }
    while (size > 0);
    file.close();
    if (fileSize == 0)
        throw(new Exception("Error while receiving message", RECV_EXCEPTION, errno)); //ErrorHandler
    else return fileSize;
}

int FtpClient::GetSocket()
{
    return _Socket;
}

int FtpClient::List(char *dir)
{
    if (_CmdSocket == -1)
        return -1;
    try
    {
        char buffer[4096];
        pthread_cancel(msgThread);
	if(!CreateThread())
	{
	  cout << "thread created\n";
	}
	else cout << "thread wasn't created\n";
	usleep(2000);
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
            perror("[CmdClient]Eroare la connect().\n");
            return errno;
        }
        int tmp = _Socket;
        char tmpDir[4096];
        if (strlen(dir) && strcmp(dir, "this"))
        {
            strcpy(tmpDir, "LIST ");
            strcat(tmpDir, dir);
        }
        else strcpy(tmpDir, "LIST");
        SendMessage(tmpDir);
        _Socket = _CmdSocket;
        strcpy(buffer, ReceiveMessage());
        cout << "LIST RESULT:\n" << buffer;
        bzero(Result, 4096);
        strcpy(Result, buffer);
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

int FtpClient::CreateThread()
{
    return pthread_create( &msgThread, NULL, WaitForMessage, NULL);
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
//         if (CreateThread())
//             perror("can't create thread\n");
// 	usleep(3000);
// 	pthread_cancel(msgThread);
        if (code != 250)
            return code;
        else
            return 0;
    }
    catch (Exception ex)
    {
        cout << ex.Message;
        return ex.ErrorCode;
    }
}


int FtpClient::Login()
{
    try
    {
        char buffer[2048];
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
	Sock = _Socket;
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

