#include "FtpClient.h"
#include <stdlib.h>




FtpClient *client;
char localPath[256], remotePath[256];

int LoadSettings ( char *file )
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
    }
    f.close();
    return 0;
}

void ShowHelp()
{
    cout << "HELP\n";
    cout << "Usage ftpmirror [OPTIONS]\n\n";
    cout << "Downloads a directory from a FTP server\n";
    cout << "-h\t\tHOSTNAME OR IP\n";
    cout << "-u\t\tUsername\n";
    cout << "-p\t\tPassword\n";
    cout << "-port\t\tPort\n";
    cout << "-l\t\tLocalPath\n";
    cout << "-r\t\tRemoteDirectory\n";
    cout << "-c\t\tConfigFile\n";
    cout << "\t\tThe config will have each option on one line\n followed by '=' and the value for that option\n";
    cout << "\tExample:\n\n";
    cout << "\tAddress=ftp.server.com\n";
    cout << "\tUsername=anonymous\n";
    cout << "\tThe file is case sensitive\n\n\n";
}

int main ( int argc, char **argv )
{
    client= new FtpClient ();//"users9.nofeehost.com"

    if ( argc > 1 )
    {
        for ( int i = 1; i < argc; i++ )
        {
	    if(!strcmp(argv[i], "--help"))
	    {
	      ShowHelp();
	      return 0;
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
    }/*
    cout<<"\nAddress="<<client->GetHostName();
    cout<<"\nIp="<<client->GetIp();
    cout<<"\nUser="<<client->GetUsername();
    cout<<"\nPassword="<<client->GetPassword();
    cout<<"\nLocalPath="<<localPath;
    cout<<"\nRemotePath="<<remotePath<<endl;*/
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
    cout << "Starting download\n";
    if ( strlen ( remotePath ) == 0 )
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
      if(!client->mkdirs(tmpDir, localPath)) 
	cout << "\ndirectory was created\n";
      else cout << "\ndirectory wasn't created\n";
    }
    if ( client->DownloadFolder ( remotePath, localPath ) == 0 )
        cout << "\nDownload succesfull\n";
    else cout << "\nError encountered while downloading folders\n";
    client->CloseConnection();
}







