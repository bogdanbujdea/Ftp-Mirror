#include "FtpClient.h"
#include <stdlib.h>




FtpClient *client;

int main(int argc, char **argv)
{
    client= new FtpClient("users9.nofeehost.com");
    client->SetUserName("bogdanftp");
    client->SetPassword("bogdan12");
    //client->SetPort(21);
    client->ConnectToServer();
    //pthread_t thread1, thread2;
    char buffer[256];
    int  iret1, iret2;
    int code;
    code = client->Login();
    if (code != 0)
    {
        cout << "Failed to login to server, error code is" << code << endl;
        exit(1);
    }
    else cout << "Login succesfull" << endl;
    iret1 = pthread_create( &client->msgThread, NULL, WaitForMessage, NULL);
    //client->List();
    int opt = 0;
    char dir[2048];
    while (1)
    {
        cout << "\n1.List directories";
        cout << "\n2.Change directory";
        cout << "\n3.Download file";
        cout << "\n4.Download folder";
        cout << endl;
        cin>>opt;
        switch (opt)
        {
        case 1:
            cout << "Enter the name of the folder:";
            scanf("%s", &dir);
            client->List(dir);

            break;
        case 2:
            cout << "Enter the name of the folder:";
            scanf("%s", &dir);
            client->ChangeDir(dir);
            break;
        case 3:
            cout << endl;
            scanf("%s", &dir);
            client->DownloadFile(dir,dir);

            break;
        case 4:
            cout << "starting download\n";
            if(client->DownloadFolder("/bogdanftp" , "/home/bogdan") == 0)
	      cout << "download succesfull\n";
	    else cout << "\nerror while dld folders\n";
            break;
        default:
            cout << "wrong choice";
        }
        pthread_create( &client->msgThread, NULL, WaitForMessage, NULL);
        sleep(1);
    }
}







