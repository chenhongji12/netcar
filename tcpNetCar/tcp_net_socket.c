#include "tcp_net_socket.h"
int tcp_init(const char* ip, int port)
{
	//服务器的tcp初始化，socket->bind->listen
	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sfd == -1)
	{
		perror("socket");
		exit(-1);
	}
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(struct sockaddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
//	serveraddr.sin_addr.s_addr = inet_addr(ip);  //or INADDR_ANY
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);  //or INADDR_ANY

	if(bind(sfd, (struct sockaddr*)&serveraddr, sizeof(struct sockaddr)) == -1)
	{
		perror("bind");
		close(sfd);
		exit(-1);
	}

	if(listen(sfd, 10) == -1)  //the maximum of connected 10, without blocking, just return for a minute
	{
		perror("bind");
		close(sfd);
		exit(-1);
	}
	return sfd;  //return the socket when succeed
}
int tcp_accept(int sfd)
{
	struct sockaddr_in clientaddr;
	memset(&clientaddr, 0, sizeof(struct sockaddr));
	int addrlen = sizeof(struct sockaddr);
	int new_fd = accept(sfd, (struct sockaddr*)&clientaddr, &addrlen);  //listen socket/ client address/ length   blocking
	//new_fd为阻塞接收连接后，建立的新socket，与sfd无关
	if(new_fd == -1)
	{
		perror("accept");
		close(sfd);
		exit(-1);		
	}
	printf("%s %d sucess connect...\n", inet_ntoa(clientaddr.sin_addr),
										ntohs(clientaddr.sin_port));
	return new_fd;
}
int tcp_connect(const char* ip, int port)   //客户端连接，服务端的地址
{
	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sfd == -1)
	{
		perror("socket");
		exit(-1);
	}
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(struct sockaddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	serveraddr.sin_addr.s_addr = inet_addr(ip);  //or INADDR_ANY

//	printf("connect前\n");
	int ret = connect(sfd, (struct sockaddr*)&serveraddr, sizeof(struct sockaddr));
	printf("%d\n", ret);
	if(ret == -1)
		perror("connect");
/*	if(connect(sfd, (struct sockaddr*)&serveraddr, sizeof(struct sockaddr)) == -1)
	{
		perror("connect");
		close(sfd);
		exit(-1);
	}*/
//	printf("connect后\n");
	return sfd;
}
/*
void signalhandler(void)
{
	sigset_t sigSet;
	sigemptyset(&sigSet);
	sigaddset(&sigSet, SIGINT);
	sigaddset(&sigSet, SIGOUT);
	sigprocmask(SIG_BLOCK, &sigSet, NULL);
}*/
