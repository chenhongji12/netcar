#include <pthread.h>
#include <unistd.h>
#include <termios.h>
#include "tcp_net_socket.h"

#define BUFLEN 1024
char buf[BUFLEN];
void *handleClient(void *arg)
{
	int cfd = *((int*)arg);
	printf("pthread accessed!\n");
	
        struct termios stored_settings;
        struct termios new_settings;
        tcgetattr(0, &stored_settings);
        new_settings = stored_settings;
        new_settings.c_lflag &= ~(ECHO|ICANON);
//        new_settings.c_cc[VTIME]=0;
//	new_settings.c_cc[VMIN] = 1;
//        tcsetattr(0, TCSANOW, &new_settings);
        tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);

    char ch=0;
    int size=0;
	while(1)
	{
		ch=getchar();
		write(cfd, &ch, 1);

		memset(buf, 0, BUFLEN);
		size = read(cfd, buf, BUFLEN);
		if(size > 0)
			puts(buf);
		else
		{
			close(cfd);
			return 0;
		}
//	write(cfd, "Heello!", 7);
//		memset(buf, 0, 1024);
//		size = read(cfd, buf, 1024);
//		if(size >0)
//			puts(buf);
//		if(size == 0)
//		{
//			close(cfd);
//			printf("close unnormally\n");
//			tcsetattr(0, TCSANOW, &stored_settings);
//			return 0;
//		}
	}
//	close(cfd);
}


int main()
{
//	int sfd = tcp_init("119.23.240.131", 8890);
	int sfd = tcp_init("", 8890);
	int ret;

	printf("listen!\n");
	while(1)
	{
		int cfd = tcp_accept(sfd);
		printf("Client accessed!\n");
		pthread_t clientPID;
		ret = pthread_create(&clientPID, NULL, handleClient, (void *)(&cfd));

/*
		while(1){
			int size=recv(cfd, buf, sizeof(buf), 0);
		if(size == -1)
		{
			perror("recv");
			close(cfd);
			close(sfd);
			exit(-1);
		}
		if(size >0)
			puts(buf);
		if(size ==0)
		{
			close(cfd);
			break;
		}
	}*/
	}
}
