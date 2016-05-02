#include <iostream>
#include <winsock2.h>
#include <time.h> 

using namespace std;
#define LISTEN_PORT 8097
#define MAX_BUFF_SIZE 	1024
#define MAX_HEADER_SIZE 1024
int createtime(char * sendtime){
	time_t timep;
	struct tm *p;
	time(&timep);
	p = gmtime(&timep);
	char wday[7][4] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
	char mon[12][4]  = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	sprintf(sendtime,"%s, %d %s %d %d:%d:%d GMT",wday[p->tm_wday],p->tm_mday,mon[p->tm_mon],p->tm_year,p->tm_hour,p->tm_min,p->tm_sec);
}
void createHeader(char *Header,int html_len){
	char sendtime[100];
	char sendBuf[MAX_BUFF_SIZE];

	memset(Header,0,sizeof(Header));
	memset(sendBuf,0,sizeof(sendBuf));
	memset(sendtime,0,sizeof(sendtime));
	
	sprintf(Header,"%s","HTTP/1.0 200 OK\r\nConnection:Close\r\nContent-Type:textml;charset=ISO-8859-1\r\n");
	createtime(sendtime);
	sprintf(Header,"%sDate:%s\r\n%s",Header,sendtime,"Server:Mooran`s Server\r\n");
	sprintf(Header,"%sContent-Length:%d\r\n\r\n",Header,html_len);
} 
int Response(char * header,int client_socket){
    const char html[100] = "<html><head><h1>Runing</h1></head><html>";
    char Header[MAX_HEADER_SIZE];
	createHeader(Header,strlen(html));
	send(client_socket,Header,strlen(Header),0);
	send(client_socket,html,strlen(html),0);
}
int initWSA(){
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested =MAKEWORD( 1, 1 );
	err = WSAStartup( wVersionRequested, &wsaData );
	return err;
}

int main(int argc, char* argv[]) {
	initWSA();
	SOCKADDR_IN server_addr,client_addr;
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(LISTEN_PORT);
	SOCKET server_socket =  socket(AF_INET,SOCK_STREAM,0);
	int err = bind(server_socket,(SOCKADDR *)&server_addr,sizeof(SOCKADDR));
	listen(server_socket,5);
	int len=sizeof(SOCKADDR);
	int client_sockets[100];
	int client_sockets_max=0;
	fd_set all_read_fds,readable_fds;
	FD_ZERO(&all_read_fds);  
	FD_SET(server_socket,&all_read_fds);
	while(1){
		readable_fds = all_read_fds;
		int recode = select( 0 , &readable_fds , 0 , 0 , 0 );
		if (recode==0){
			cout<<"time out"<<endl;
		}else{
			if (FD_ISSET(server_socket,&readable_fds)){
				SOCKET client_socket = accept(server_socket,(SOCKADDR *)&client_addr,&len);
				client_sockets[client_sockets_max ++] = client_socket;//TODO 这里有毒 暂时还没想好怎么弄 
				FD_SET(client_socket,&all_read_fds);
				cout<<client_socket<<"Connecting"<<endl;
			}else{
				for (int i = 0;i < client_sockets_max;i++){
					if (client_sockets[i]<0)continue;
					if (FD_ISSET(client_sockets[i],&readable_fds)){
						char recv_buff[1024];
						int data_length = recv(client_sockets[i],recv_buff,sizeof(recv_buff),0);
						if (data_length == 0 ){//连接断开 
							FD_CLR( client_sockets[i] , &all_read_fds );
							closesocket(client_sockets[i]);
							cout<<client_sockets[i]<<"disconnected"<<endl;
							client_sockets[i] = -1;
						}else{
							Response(recv_buff,client_sockets[i]);
						}
					}
				}
			}
		}
	}
	closesocket(server_socket);
	WSACleanup();
	return 0;
}
