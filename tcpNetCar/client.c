#include <stdio.h>
#include "tcp_net_socket.h"

#include "pwm.h"

char buffer[1024];
//ssize_t size = 0;

#define MOTORMEDIAL 500
#define SERVOMEDIAL 500
int pwmData0=MOTORMEDIAL;
int pwmData1=SERVOMEDIAL;
int scaleNUM=1;
int heartbeatFlag=0;

#define scalePWM(a)		1280+a*0.8  //1.4ms = 1680
char buf[1024];
void servoStop()  //cannot set a value in a large interval
{
/*		pwmData0= INTERVALUE;
		pwmData(0, scalePWM(pwmData0) );*/
		int i, differValue, polariFlag;
		differValue = pwmData0-MOTORMEDIAL;
		polariFlag =1;
		if(differValue<0)
		{
				differValue = SERVOMEDIAL-pwmData0;
				polariFlag = 0;
		}
		for(i=0; i<differValue; i++)
		{
				usleep(10);
				if(polariFlag>0)
				{
						pwmData0--;
				}
				else
				{
						pwmData0++;
				}
				pwmData(0, scalePWM(pwmData0) );
		}
}

int servoPwm(int sfd, char mode)
{
	switch(mode){
		case '1':
			scaleNUM = 1;
			break;
		case '2':
			scaleNUM = 10;
			break;
		case '3':
			scaleNUM = 50;
			break;
		case 'w':
			pwmData0 -= scaleNUM;
			break;
		case 's':
			pwmData0 += scaleNUM;
			break;
		case 'a':
			pwmData1 -= scaleNUM;
			break;
		case 'd':
			pwmData1 += scaleNUM;
			break;
		case '0':
			heartbeatFlag = 1;
			break;
		case 'q':
			servoStop();
			break;
		case 'e':
			pwmData(1, SERVOMEDIAL);
		default:
			break;
	}
	if(pwmData0>1000)
		pwmData0=1000;
	if(pwmData0<0)
		pwmData0=0;
	if(pwmData1>1000)
		pwmData1=1000;
	if(pwmData1<0)
		pwmData1=0;

	pwmData(0, scalePWM(pwmData0) );
	pwmData(1, scalePWM(pwmData1) );
/*	if((mode=='w')||(mode=='s'))
	{
			int size=0;
			int ret=0;
			memset(buf, 0, 1024);
			size = sprintf(buf, "motor speed:%d\n", pwmData0);
			puts(buf);
			ret = write(sfd, buf, size);
			if(ret<=0)
			{
					close(sfd);
					return 0;
			}
			return pwmData0;
	}
	if((mode=='a')||(mode=='d'))
	{
			int size=0;
			int ret=0;
			memset(buf, 0, 1024);
			size = sprintf(buf, "servor speed:%d\n", pwmData1);
			puts(buf);
			ret = write(sfd, buf, size);
			if(ret<=0)
			{
					close(sfd);
					return 0;
			}
			return pwmData1;
	}
	if(mode=='q')
	{
*/
	int size=0;
	memset(buf, 0, 1024);
	switch(mode){
			case 'w':
			case 's':
				size = sprintf(buf, "motor speed:%d\n", 1000-pwmData0);
				break;
			case 'a':
			case 'd':
				size = sprintf(buf, "servo speed:%d\n", pwmData1);
				break;
			case '1':
			case '2':
			case '3':
				size = sprintf(buf, "interval ratio:%c\n", mode);
				break;
			case 'q':
				size = sprintf(buf, "motor stop!\n");
				break;
			case 'e':
				size = sprintf(buf, "servo return!\n");
				break;
			default:	
				size = sprintf(buf, "input error!\n");
				break;
	}
	int ret = write(sfd, buf, size);
	if(ret <= 0)
	{
			close(sfd);
			return 0;
	}
	return 1;
}

int main()
{
	int sfd;
	int ret;
	int size;
	sfd = tcp_connect("119.23.240.131", 8890);
//	sfd = tcp_connect("192.168.1.103", 8890);

	pwmInit();

	pwmData(0, 1680);
//	send(sfd, "hello", 6, 0);
//	printf("不阻塞\n");

	while(1)
	{
		
		size = read(sfd, buf, 1);
		if(size==0)
		{
			pwmData(0, 1680 );
//			servo(sfd, 'q');
			close(sfd);
			return -1;
		}
		if(size >0)
			ret = servoPwm(sfd, buf[0]);	

	}
}
