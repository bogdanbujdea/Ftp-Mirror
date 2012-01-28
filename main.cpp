#include "FtpClient.h"
#include <stdlib.h>


void *WaitForMessage( void *ptr );

FtpClient *client;

int main(int argc, char **argv)
{
    client= new FtpClient("ftp.kde.org");
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
        cout << endl;
        cin>>opt;
        switch (opt)
        {
        case 1:
            cout << "Enter the name of the folder:";
            scanf("%s", &dir);
            client->List(dir);
            iret1 = pthread_create( &client->msgThread, NULL, WaitForMessage, NULL);
            break;
        case 2:
            cout << "Enter the name of the folder:";
            scanf("%s", &dir);
            client->ChangeDir(dir);
            iret1 = pthread_create( &client->msgThread, NULL, WaitForMessage, NULL);
            break;
        default:
            cout << "wrong choice";
        }
        sleep(1);
    }
    printf("Thread 1 returns: %d\n",iret1);
    //printf("Thread 2 returns: %d\n",iret2);
}



void *WaitForMessage( void *ptr )
{
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
    char buffer[256];
    int n;
    while (1)
    {
        cout << endl << client->ReceiveMessage() << endl;
    }
}
