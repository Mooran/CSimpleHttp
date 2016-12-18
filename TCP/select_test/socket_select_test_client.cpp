#include <sys/types.h>   
#include <sys/socket.h>   
#include <stdio.h>   
#include <netinet/in.h>                                                                      
#include <arpa/inet.h>   
#include <unistd.h>   
#include <string.h>
int main()   
{   
    int client_sockfd;   
    int len;   
    struct sockaddr_in address;//服务器端网络地址结构体
     int result;   
    char ch[1024];   
    client_sockfd = socket(AF_INET, SOCK_STREAM, 0);//建立客户端socket                                 
    address.sin_family = AF_INET;   
    address.sin_addr.s_addr = inet_addr("127.0.0.1");               
    address.sin_port = htons(9734);   
    len = sizeof(address);   
    result = connect(client_sockfd, (struct sockaddr *)&address, len);   
    if(result == -1)   
    {   
         perror("oops: client2");   
         return 1;   
    }   
    while(1){
        scanf("%s",ch);
        write(client_sockfd, ch,strlen(ch)+1);   
        read(client_sockfd, ch, sizeof(ch));   
        printf("response :  %s\n", ch);
    }
    close(client_sockfd);   
   return 0;
}    