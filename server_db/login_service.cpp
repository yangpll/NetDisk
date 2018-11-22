/*************************************************************************
    > File Name: login_service.cpp
    > Author: yy
    > Mail: yy@ityy520.com 
    > Created Time: Wed 21 Nov 2018 08:14:06 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<string>
#include<string.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<sys/stat.h>
#include<pthread.h>
#include<signal.h>
#include<unistd.h>

#pragma pack(1)
typedef struct _usr_info
{
	 char Usr[33];
	 char passwd[33];
}usr_info;
#pragma pack()
void* thr_login(void* arg);

int main()
{
	int sock_listen;

	sock_listen = socket(AF_INET,SOCK_STREAM,0);//监听套I接字

	int optval = 1;
	setsockopt(sock_listen,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));

	struct sockaddr_in myaddr;
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = INADDR_ANY;
	myaddr.sin_port = htons(11111);//端口号

	if(bind(sock_listen,(struct sockaddr*)&myaddr,sizeof(myaddr)))//绑定
	{
		perror("bind error");
		exit(1);
	}

	listen(sock_listen,10);
	printf("登录服务启动成功，运行中!\n");

	struct sockaddr_in cliaddr;
	socklen_t len;

	while(1)
	{
		int sock_conn;//连接套接字

		signal(SIGPIPE,SIG_IGN);
		len = sizeof(cliaddr);
		sock_conn = accept(sock_listen,(struct sockaddr*)&cliaddr,&len);

		if(sock_conn == -1)
		{
			perror("accept error");
			continue;
		}


		pthread_t tid;
		if(pthread_create(&tid,NULL,thr_login,(void*)(long)sock_conn))
		{
			perror("create new thread fail");
			close(sock_conn);
			continue;
		}
	}

	close(sock_listen);

	return 0;
}


void* thr_login(void* arg)
{
	pthread_detach(pthread_self());//线程结束自动释放空间

	int sock_conn = (long)arg; //传入套接字
	struct sockaddr_in cliaddr;
	socklen_t len = sizeof(cliaddr);
	getpeername(sock_conn,(struct sockaddr*)&cliaddr,&len);

	printf("\n客户端%s:%d已经连接!\n",inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));
	usr_info info;
	//char* buff = new char[1024];

	int ret;
	ret = recv(sock_conn,&info,sizeof(info),0);
	
	printf("ret = %d\n",ret);
	
	printf("\n客户端%s:%d已经下线\n",inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));

}
