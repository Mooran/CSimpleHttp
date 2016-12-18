#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#define MAX_SEND_CACHE 1024

int main(){

    int mysock = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in destination_addr;

    destination_addr.sin_family = AF_INET;
    destination_addr.sin_port = htons(8888);
    destination_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(mysock,(sockaddr *)&destination_addr,sizeof(destination_addr));
    char send_buff[MAX_SEND_CACHE]="Hello";
    send(mysock,send_buff,strlen(send_buff),0);
    send(mysock,send_buff,strlen(send_buff),0);
    return 0;
}
