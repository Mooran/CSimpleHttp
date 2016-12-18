#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
int main(){
    int server_sockfd, client_sockfd;
    socklen_t server_len, client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    int result;
    fd_set all_read_check_fds;

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);//建立服务器端socket
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(9734);
    server_len = sizeof(server_address);
    int err = bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
    listen(server_sockfd, 5);

    FD_ZERO(&all_read_check_fds);
    FD_SET(server_sockfd, &all_read_check_fds);//将服务器端socket加入到集合中  

    printf("sockfd:%d\nerr:%d\n",server_sockfd,err);

    while(1) {
        printf("server waiting isset:%d \n",FD_ISSET(server_sockfd,&all_read_check_fds));
        fd_set can_read_fds = all_read_check_fds;
        result = select(10, &can_read_fds, 0 , 0 , 0);
        for(int fd = 0; fd < 10; fd++){
            if(FD_ISSET(fd,&can_read_fds)){
                 if(fd == server_sockfd){
                    client_len = sizeof ( client_address ) ;
                    client_sockfd = accept ( server_sockfd,(struct sockaddr *)&client_address, &client_len );
                    FD_SET(client_sockfd, &all_read_check_fds);	//将客户端socket加入到集合中
                    printf("adding client on fd %d\n", client_sockfd);
                 }else{
                    int nread;
                    ioctl(fd, FIONREAD, &nread);//取得数据量交给nread 
                    if(nread == 0)
                    {
                        FD_CLR(fd, &all_read_check_fds);
                        close(fd);
                        printf("removing client on fd %d\n", fd);
                    }else{
                        char ch[1024];
                        read(fd, ch, 1024);
                        printf("serving client on fd %d\n:content:%s\n", fd,ch);
                        write(fd, ch, strlen(ch)+1);
                    }
                }
            }
        }
    }
} 