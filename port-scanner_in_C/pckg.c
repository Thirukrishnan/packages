#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <unistd.h>
#include<pthread.h>
#include<unistd.h>

#define MAX 200
int no_of_open_ports;
int open_ports[MAX];
int closed_ports;
int job=1;

struct arguments
{
	int start;
	int end;
	struct sockaddr_in sa;
};
void *scanner(void *args)
{
	int sock,err;
	struct arguments *args_ori=(struct arguments *)args;

	int start=args_ori->start;
	int end=args_ori->end;
	struct sockaddr_in sa=args_ori->sa;

	for(int i=start;i<=end;i++)
	{
		sa.sin_port = htons(i);
		sock = socket(AF_INET , SOCK_STREAM , 0);
		if(sock < 0)
		{
			perror("\nSocket");
			exit(1);
		}

		err = connect(sock , (struct sockaddr*)&sa , sizeof sa);

		if( err >= 0 )
		{
			no_of_open_ports++;
			open_ports[no_of_open_ports-1]=i;
			fflush(stdout);
		}
		else
		{
			closed_ports++;
		}
		close(sock);
	}
	job++;
}
int main(int argc , char **argv)
{
	struct hostent *host;
	int  i , start , end;
	char hostname[100];
	struct sockaddr_in sa;

	struct arguments var;
	pthread_t threads[10];

	printf("Enter host to scan:");
	gets(hostname);
	printf("Enter range of port-numbers:\n");
	printf("Start:");
	scanf("%d" , &start);
	printf("Last:");
	scanf("%d" , &end);
	

	strncpy((char*)&sa , "" , sizeof sa);
	sa.sin_family = AF_INET;

	if(isdigit(hostname[0]))
	{
		printf("Doing inet_addr...");
		sa.sin_addr.s_addr = inet_addr(hostname);
		printf("Done\n");
	}
	else if( (host = gethostbyname(hostname)) != 0)
	{
		printf("Doing gethostbyname...");
		strncpy((char*)&sa.sin_addr , (char*)host->h_addr , sizeof sa.sin_addr);
		printf("Done\n");
	}
	else
	{
		herror(hostname);
		exit(2);
	}
	var.sa=sa;

	printf("Starting the portscan loop : \n");

	int total_ports=end-start+1;
	int ports_per_thread= (total_ports/10);
	if(ports_per_thread>0)
	{
		printf("\nTotal ports: %d\t\t",total_ports);
		printf("Ports per thread is :%d\n\n",ports_per_thread);
	}
	else
	{
		printf("\nTotal ports: %d\t\t",total_ports);
		 printf("Ports per thread is :%d\n\n",1);
	}
	int j=start;

	for(int i=0 ; i<10 && j<=end ;i++)
	{
		var.start=j;
		var.end=j+ports_per_thread;
		j=var.end+1;
		
		printf("Thread %d scans port in range (%d,%d)\n",i+1,var.start,var.end);
		pthread_create(&threads[i],NULL,scanner,(void *)&var);
		pthread_join(threads[i],NULL);

	}

	printf("\n\tNumber of open ports:%d\n\n",no_of_open_ports);
	printf("\t\tPort State\n");
	for(int i=0;i<no_of_open_ports;i++)
		printf("\t\t%-5d open\n",open_ports[i]);
		//printf("Open ports  %d\n",open_ports[0]);
	printf("\r");
	fflush(stdout);
	return(0);
}
