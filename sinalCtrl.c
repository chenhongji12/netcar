#include <stdio.h>
#include <sigal.h>
#include <unistd.h>

void Stop(void)
{
	printf("access ctrl+c!");
	_exit(1);
}
int main()
{
	int res;

	signal(SIGINT, Stop);
	while(1);
	return 0;
}
