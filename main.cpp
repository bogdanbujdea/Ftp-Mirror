
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
#include <stdlib.h>

FtpClient *client;
char localPath[256], remotePath[256];

int LoadSettings ( char *file ) //loads setting from a configuration file
{
    ifstream f ( file, ios::in );
    if ( !f.is_open() )
        return errno;
    string settings[10];
    char line[256], value[256], key[256];
    bzero ( line, 256 );
    int i = 0;

    while ( !f.eof() )
    {
        bzero ( line, 256 );
        f.getline ( line, 256 );
        //settings[i] = line;
        bzero ( value, 256 );
        bzero ( key, 256 );
        for ( i = 0; line[i] != '=' && i < strlen ( line ); key[i] = line[i++] );
        key[i] = '\0';
        int j = 0;
        for ( i = i + 1; i < strlen ( line ); value[j++] = line[i++] );
        value[j] = '\0';

        if ( strlen ( line ) == 0 )
            break;
        
        if ( !strcmp ( key, "Address" ) )
        {
            client->SetHostName ( value );
        }
        else if ( !strcmp ( key, "User" ) )
            client->SetUserName ( value );
        else if ( !strcmp ( key, "Password" ) )
            client->SetPassword ( value );
        else if ( !strcmp ( key, "LocalPath" ) )
            strcpy ( localPath, value );
        else if ( !strcmp ( key, "RemotePath" ) )
            strcpy ( remotePath, value );
        else if ( !strcmp ( key, "Port" ) )
        {
            int port = atoi ( value );
            client->SetPort ( port );
        }
        else if (!strcmp(key, "Refresh"))
	{
	  int tmp = atoi(value);
	  client->Refresh = tmp;
	}
    }
    f.close();
    return 0;
}

void ShowHelp()
{
  char *license="<FtpMirror>  Copyright (C) <2012>  <Bujdea Bogdan>\n"
    "This program comes with ABSOLUTELY NO WARRANTY\n"
    "This is free software, and you are welcome to redistribute it\n"
    "under certain conditions; press 'Enter' for details.\n\n";
    cout << license;
    cout << "\nPress Enter to continue...";
    getchar();
    cout << "HELP\n";
    cout << "Usage ftpmirror [OPTIONS]\n\n";
    cout << "Downloads a directory from a FTP server\n";
    cout << "-h\t\tHOSTNAME OR IP\n";
    cout << "-u\t\tUsername\n";
    cout << "-p\t\tPassword\n";
    cout << "-port\t\tPort\n";
    cout << "-l\t\tLocalPath\n";
    cout << "-r\t\tRemoteDirectory\n";
    cout << "-refresh\t\tRefresh directory\n";
    cout << "-c\t\tConfigFile\n";
    cout << "\t\tThe config will have each option on one line\n followed by '=' and the value for that option\n";
    cout << "\tExample:\n\n";
    cout << "\tAddress=ftp.server.com\n";
    cout << "\tUsername=anonymous\n";
    cout << "\tThe file is case sensitive\n\n\n";
}

int main ( int argc, char **argv )
{
    client= new FtpClient ();
    int Refresh = 0;
    if ( argc > 1 )
    {
        for ( int i = 1; i < argc; i++ )
        {
	    if(!strcmp(argv[i], "--help"))
	    {
	      ShowHelp();
	      return 0;
	    }
	    if(!strcmp(argv[i], "-refresh"))
	    {
		Refresh = 1;
		break;
	    }
            if ( !strcmp ( argv[i], "-c" ) )
            {
                LoadSettings ( argv[i+1] );
                break;
            }
            if ( !strcmp ( argv[i], "-h" ) )
            {
                client->SetHostName ( argv[i+1] );
                printf ( "host[%d]=%s   argv[%d]=%s", i, argv[i], i+1, argv[i+1] );
                continue;
            }
            
            if ( !strcmp ( argv[i], "-p" ) )
            {
                client->SetPassword ( argv[i+1] );
                printf ( "pass[%d]=%s   argv[%d]=%s", i, argv[i], i+1, argv[i+1] );
                continue;
            }
            if ( !strcmp ( argv[i], "-port" ) )
            {
                int port = atoi ( argv[i+1] );
                printf ( "port[%d]=%s   argv[%d]=%s", i, argv[i], i+1, argv[i+1] );
                client->SetPort ( port );
                continue;
            }
            if ( !strcmp ( argv[i], "-u" ) )
            {
                client->SetUserName ( argv[i+1] );
                printf ( "user[%d]=%s   argv[%d]=%s", i, argv[i], i+1, argv[i+1] );
                continue;
            }
            if ( !strcmp ( argv[i], "-l" ) )
            {
                strcpy ( localPath, argv[i+1] );
                printf ( "localpath[%d]=%s   argv[%d]=%s", i, argv[i], i+1, argv[i+1] );
                continue;
            }
            if ( !strcmp ( argv[i], "-r" ) )
            {
                strcpy ( remotePath, argv[i+1] );
                printf ( "remotepath[%d]=%s   argv[%d]=%s", i, argv[i], i+1, argv[i+1] );
                continue;
            }
        }
    }
    else
    {
        ShowHelp();
        return 1;
    }
    if ( strlen ( localPath ) == 0 )
    {
        cout << "The files will be downloaded on /home";
        cout << "\nConsider using -l for local path";
        strcpy ( localPath, "/home" );
    }

    if ( strlen ( client->GetIp() ) == 0 )
    {
        cout << "You must provide a ftp server using -h option";
        exit ( 1 );
    }
    int code = 0;
    if ( client->ConnectToServer() )
    {
        cout << "\nConnection Failed\n";
        exit ( 1 );
    }
    code = client->Login();
    if ( code != 0 )
    {
        cout << "Failed to login to server, error code is" << code << endl;
        exit ( 1 );
    }
    else cout << "Login succesfull" << endl;
    if ( strlen ( remotePath ) == 0 ) //if a remote path won't be specified, then the client will get the current directory from the server
    {
        cout << "The root will be specified as default, but it may not work if you don't have the permission";
        cout << "\nConsider using -r for remote directory";
        strcpy ( remotePath, client->GetCurrentDirectory());
    }
    else
    {
      char tmpDir[1024];
      strcpy(tmpDir, remotePath); /*remotePath trebuie copiat intr-o variabila si trimis ca parametru
				    altfel caracterul '/' se va schimba in NULL dupa functia strtok din mkdirs()*/
      if(client->mkdirs(tmpDir, localPath)) 
	cout << "\ndirectory wasn't created\n";
    }
    
    if ( client->DownloadFolder ( remotePath, localPath ) == 0 )
        cout << "\nDirectory download succesfull! The program will now exit\n";
    else cout << "\n\n\nError encountered while downloading folders. The program will now exit\n";
    client->CloseConnection();
    sleep(5);
    exit(0);
}







