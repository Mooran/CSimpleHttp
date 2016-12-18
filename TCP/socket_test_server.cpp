#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define MAX_RECV_CACHE 5

int main(){

    int listen_socket = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in local_bind_addr;
    struct sockaddr_in client_conn_addr;
    local_bind_addr.sin_family = AF_INET;
    local_bind_addr.sin_port    = htons(8888);
    local_bind_addr.sin_addr.s_addr   = htonl(INADDR_ANY);

    int err = bind(listen_socket,(sockaddr *)&local_bind_addr,sizeof(local_bind_addr));
    listen(listen_socket,5);
    socklen_t len = sizeof(client_conn_addr);
    while(1){
        int conversation_socket = accept(listen_socket,(sockaddr *)&client_conn_addr,&len);
        while(1){
            char recv_buff[MAX_RECV_CACHE+1]="";
            int recv_length = recv(conversation_socket,recv_buff,MAX_RECV_CACHE,0);
            if (recv_length == 0){
            	    printf("disconnect\n");
            	    break;
            }else{
                 printf("data_length: %d\ndata_content:%s\n",recv_length,recv_buff);
            }
        }
    }
    return 0;
}
