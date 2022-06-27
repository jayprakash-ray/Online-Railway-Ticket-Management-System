int authentication(struct loginDetails);
void clientAction(int socket)
{
	struct loginDetails login;
	//int* cli=(int *)sock;
	//int socket=*cli;
	printf("IN CLIENT ACTION");
	read(socket,&login,sizeof(login));
	printf("\nUSERTYPE:%d",login.userType);
	
	switch(login.userType)
	{
		case 1:
			printf("\nCUSTOMER SIGNING IN.....");
			int status=authentication(login);
			send(socket,&status,1,0);
			if(status==1)
			{
				customerActions(socket);
			}
			//Customer
			break;
		case 2:
			//Admin
			printf("\nADMIN SIGNING IN.....");
			status=authentication(login);
			//printf("Returned from Auth :%d",status);
			send(socket,&status,1,0);
			if(status==1)
			{
			adminActions(socket);
			}
			break;
		case 3: 
			printf("\nAGENT SIGNING IN.....");
			status=authentication(login);
			send(socket,&status,1,0);
			if(status==1)
			{
				agentActions(socket);
			}
			//Customer
			break;
	}
}

//void adminActions(int clientSock,int uid);

int authentication(struct loginDetails login)
{
	printf("IN AUTHENTICATION FUNCTION");
	if(login.userId==100 && !strcmp(login.password,"admin") && login.userType==2)
	{
		return 1;
	}
	int fd=open("ACCOUNTS.txt",O_RDWR);
	
	if(fd==-1)
	{
		perror("OPEN");
		exit(EXIT_FAILURE);
	}
	int found=0,pos=0;
	struct accounts user;
	while(read(fd,&user,sizeof(user))!=0)
	{
		pos++;
		printf("\n%d,%d",user.userId,login.userId);
		if(user.userId==login.userId)
		{
			
			found=1;
			break;
		}
		
	}
	pos--;
	//printf("\nPOS= %d,FOUND:%d",pos,found);
	struct flock lock;
	lock.l_type=F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=pos*sizeof(login);
	lock.l_len=sizeof(login);
	lock.l_pid=getpid();
	int lk=fcntl(fd,F_SETLKW,&lock);
	if(lk==-1)
	{
		perror("LOCK");
	}
	if(found)
	{
		//printf("%s,%s,%d,%d",user.password,login.password,login.userType,user.userType);
		if(user.userId==login.userId && !strcmp(user.password,login.password) && login.userType==user.userType)
		{
			return 1;
		}
		else
			return 0;
	}
	else
		return 0;
	
}	
