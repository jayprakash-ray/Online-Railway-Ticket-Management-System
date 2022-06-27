#include<string.h>
#include "headers.h"
#include "structures.h"


void clientAction(int client)
{
	char pass[100];
	int choice,uid;
	int status=0;
	
	printf("1. CUSTOMER\n 2. ADMINISTRATOR\n 3. AGENT\n Please Select Your Role :");
	scanf("%d",&choice);
	
	printf("\n ENTER USER ID :");
	scanf("%d",&uid);
	printf("\n ENTER PASSWORD: ");
	scanf("%s",pass);
	struct loginDetails login;
	login.userId=uid;
	login.userType=choice;
	strcpy(login.password,pass);
	
	send(client,&login,sizeof(login),0);
	recv(client,&status,1,0);
	//printf("\nSTATUS RECV=%d",status);
	if(status==1)
	{
		printf("\nSIGN In SUCCESSFUL !");
		printf("\n*******************RAILWAY TICKET MANAGEMENT******************");
		switch(choice)
		{
			case 1:
				customerMenu(client,uid);
				break;
			case 2:
				adminMenu(client);
				break;
			case 3:
				agentMenu(client,uid);
				break;
		}
	}
	else
	{
		printf("\nSign In Failed! Please Check your Credentials and Try Again!\n");
	}
				
}
int main(int argc, char const *argv[])
{
	struct sockaddr_in client_addr;
	int cli_fd;
	client_addr.sin_family= AF_UNIX;
	cli_fd=socket(AF_UNIX, SOCK_STREAM ,0);
	if(cli_fd==-1)
	{
		printf("\n Socket Creation Failed");
		exit(EXIT_FAILURE);
	}
	client_addr.sin_port=htons(PORT);
	if(connect(cli_fd ,(struct sockaddr*)&client_addr,sizeof(client_addr))==-1)
	{
		printf("\n Connection Error!");
		exit(EXIT_FAILURE);
	}
	printf("\nConnected to Server on Port %d\n",client_addr.sin_port);
	clientAction(cli_fd);
	
	return 0;
}
	
	
		
