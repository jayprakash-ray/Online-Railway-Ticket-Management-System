#include "headers.h"
#include "structures.h"
#include "constants.h"
#include "serverUtil.h"

int main(int arrgc ,char* agrv[])
{
	//creating Server Side Socket
	struct sockaddr_in serverSock,clientSock;
	int serv_sz,cli_sz;
	int serv_sd,cli_sd;
	if((serv_sd=socket(AF_UNIX,SOCK_STREAM,0))==0)
	{
		printf("Socket Creation Failed\n");
		exit(EXIT_FAILURE);
	}
	serverSock.sin_family = AF_UNIX;
	serverSock.sin_addr.s_addr=INADDR_ANY;
	serverSock.sin_port =htons(PORT);
	
	serv_sz=sizeof(serverSock);
	cli_sz=sizeof(clientSock);
	//bind socket to a address
	if(bind(serv_sd,(struct sockaddr *)&serverSock,serv_sz)==-1)
	{
		printf("\nSocket Binding Failed\n");
		perror("BIND");
		exit(EXIT_FAILURE);
	}
	listen(serv_sd,5);
	printf("\nServer Listening on Port: %d for New Connection......\n",serverSock.sin_port);
	
	//Accepting and Serving Clients connection
	while(1)
	{
		cli_sd=accept(serv_sd,(void *)&clientSock,&cli_sz);
		perror("ACCEPT");
		if(cli_sd==-1)
		{
			printf("\nCannot Connect :(");
			perror("Connect");
			exit(EXIT_FAILURE);
		}
		printf("\nConnection Established with Client IP : %s \n",inet_ntoa(clientSock.sin_addr));
		if(!fork())
		{	
			close(serv_sd);
			clientAction(cli_sd);
			exit(1);
		}
		else
			close(cli_sd);
		
	}
	return 0;
}
	
	
