#include <signal.h>
#include <unistd.h>
#include<stdio.h>  
void ctrlhandler( int a )  
{  
 printf("you have press ctrl+c /n");  
 /* close 相关操作*/  
 getchar();  
 exit(0);  
}  
int main()  
{  
 signal(SIGINT,ctrlhandler);  
 /*其他操作*/  
 getchar();  
 return 1;  
}  
