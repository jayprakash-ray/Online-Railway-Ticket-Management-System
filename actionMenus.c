#include "constants.h"
#include "structures.h"
#include "headers.h"

void adminMenu(int clientSock)
{

	
	while(1)
	{
	int choice;
	struct trainDetails train;
	struct accounts user;
	printf("\n**********************************************************************");
	printf("\n1.ADD TRAIN\n2.DELETE TRAIN\n3.MODIFY TRAIN DETAILS \n4.SEARCH TRAIN\n5.ADD ACCOUNT\n6.DELETE EXISTING ACCOUNT\n7.MODIFY USER DETAILS\n8.SEARCH USER\n9.SIGNOUT");
	printf("\n**********************************************************************");
	
	printf("\nENTER YOUR CHOICE: ");
	scanf("%d",&choice);
	/*while(choice<0 && choice >11 && !isNumeric(choice+'0'))
	{
		printf("\nInvalid Entry! Try ");
		printf("\nEnter Your Choice : ");
		scanf("%d",&choice);
	}*/
	int ch;
	int fd;
	char userType[10];
	struct serviceRequest sr;
	struct serverResponse res;
	switch(choice)
	{
		case 1: 
			printf("\nENTER TRAIN NUMBER:");
			scanf("%d",&sr.train.train_no);
			printf("\nENTER TOTAL SEATS:");
			scanf("%d",&sr.train.total_seats);
			sr.train.available_seats=sr.train.total_seats;
			sr.actionType=ADD_TRAIN;
			//printf("\nVALUE SENding FROM CLIENT");
			send(clientSock,&sr,sizeof(sr),0);
			//perror("SEND");
			read(clientSock,&res,sizeof(res));
			if(res.actionStatus==SUCCESS)
			{
				printf("\nTRAIN ADDED SUCCESSFULLY:)");
			}
			else if(res.actionStatus==ALREADYEXIST)
			{
				printf("\n TRAIN NO ALREADY EXIST:(");
			}
			else
			{
				printf("\nTRAIN ADD OPERATION FAILED:(");
			}
			break;
		case 2:
			printf("\nENTER TRAIN NUMBER TO BE DELETED:");
			scanf("%d",&sr.train.train_no);
			sr.train.available_seats=sr.train.total_seats;
			sr.actionType=DELETE_TRAIN;
			//printf("\nVALUE Sending FROM CLIENT");
			send(clientSock,&sr,sizeof(sr),0);
			//perror("SEND");
			read(clientSock,&res,sizeof(res));
			if(res.actionStatus==SUCCESS)
			{
				printf("\nTRAIN DELETED SUCCESSFULLY:)");
			}
			else if(res.actionStatus==DOESNOTEXIST)
			{
				printf("\n TRAIN NO DOES NOT EXIST:(");
			}
			else
			{
				printf("\nTRAIN DELETION FAILED:(");
			}
			break;
		case 3: 
			printf("\nENTER TRAIN NUMBER TO BE MODIFIED:");
			scanf("%d",&sr.train.train_no);
			sr.actionType=MODIFY_TRAIN;
			send(clientSock,&sr,sizeof(sr),0);
			//printf("\nSEND");
			//perror("SEND");
			
			read(clientSock,&res,sizeof(res));
			if(res.actionStatus==ALREADYEXIST)
			{
				printf("\nENTER NEW VALUE FOR TOTAL SEATS");
				scanf("%d",&sr.train.total_seats);
				sr.train.available_seats=sr.train.total_seats;
				printf("\nVALUE SENding FROM CLIENT");
				send(clientSock,&sr,sizeof(sr),0);
				perror("SEND");
				read(clientSock,&res,sizeof(res));
				if(res.actionStatus==SUCCESS)
				{
					printf("\nTRAIN DETAILS MODIFIED SUCCESSFULLY:)");
				}
				else if(res.actionStatus==DOESNOTEXIST)
				{
					printf("\n TRAIN NO DOESNOT EXIST:(");
				}
				else
				{
					printf("\nMODIFY TRAIN DETAILS OPERATION FAILED:(");
				}
			}
			else if(res.actionStatus==DOESNOTEXIST)
			{
				printf("\n TRAIN NO DOES NOT EXIST!");
			}
			else
			{
				printf("\n MODIFY OPERATION FAILED:(");
			}
			break;
		case 4:
			printf("\nENTER TRAIN NUMBER TO BE SEARCHED:");
			scanf("%d",&sr.train.train_no);
			sr.actionType=SEARCH_TRAIN;
			////printf("\nVALUE Sending FROM CLIENT");
			send(clientSock,&sr,sizeof(sr),0);
			perror("SEND");
			read(clientSock,&res,sizeof(res));
			if(res.actionStatus==SUCCESS)
			{
				printf("\n:::::::SEARCH RESULT::::::");
				printf("\nTRAIN NO\t TOTAL SEATS \t AVAILABLE SEATS");
				printf("\n%d\t     \t%d\t      \t%d",res.train.train_no,res.train.total_seats,res.train.available_seats);
			}
			else if(res.actionStatus==DOESNOTEXIST)
			{
				printf("\n TRAIN NO DOES NOT EXIST:(");
			}
			else
			{
				printf("\nTRAIN SEARCHING FAILED:(");
			}
			break;
		
		case 5: 
			printf("\nENTER USER ID:");
			scanf("%d",&user.userId);
			printf("\nENTER NAME :");
			scanf("%s",user.userName);
			printf("\nENTER PASSWORD:");
			scanf("%s",user.password);
			//user.no_of_bookings=0;
			printf("\n1.CUSTOMER\n2.ADMIN\n3.AGENT \nCHOOSE USER TYPE:");
			scanf("%d",&ch);
			while(ch<1 || ch>3)
			{
				printf("\nINVALID CHOICE!PLEASE RETRY ");
				printf("\n1.CUSTOMER \n2.ADMIN \n 3.AGENT \nCHOOSE USER TYPE:");
				scanf("%d",&ch);
			}	
			user.userType=ch;
			sr.user=user;
			sr.actionType=ADD_USER;
			//printf("\nVALUE SENding FROM CLIENT");
			send(clientSock,&sr,sizeof(sr),0);
			//perror("SEND");
			read(clientSock,&res,sizeof(res));
			if(res.actionStatus==SUCCESS)
			{
				printf("\nUSER ADDED SUCCESSFULLY:)");
			}
			else if(res.actionStatus==ALREADYEXIST)
			{
				printf("\n USER ID ALREADY EXIST:( PLEASE TRY WITH DIFFERENT ID");
			}
			else
			{
				printf("\nUSER ADD OPERATION FAILED:(");
			}
			break;
		case 6:
			printf("\nENTER USER ID TO BE DELETED:");
			scanf("%d",&sr.user.userId);
			sr.actionType=DELETE_USER;
			//printf("\nVALUE Sending FROM CLIENT");
			send(clientSock,&sr,sizeof(sr),0);
			//perror("SEND");
			read(clientSock,&res,sizeof(res));
			if(res.actionStatus==SUCCESS)
			{
				printf("\n USER DELETED SUCCESSFULLY:)");
			}
			else if(res.actionStatus==DOESNOTEXIST)
			{
				printf("\n USER DOES NOT EXIST:(");
			}
			else
			{
				printf("\nUSER DELETION FAILED:(");
			}
			break;
		case 7: 
			printf("\nENTER USER ID TO BE MODIFIED:");
			scanf("%d",&sr.user.userId);
			sr.actionType=MODIFY_USER;
			send(clientSock,&sr,sizeof(sr),0);
			read(clientSock,&res,sizeof(res));
			
			if(res.actionStatus==ALREADYEXIST)
			{
				printf("\nENTER NEW NAME :");
				scanf("%s",sr.user.userName);
				printf("USERNAME ENTERED:%s",user.userName);
				
				printf("\nENTER NEW PASSWORD:");
				scanf("%s",sr.user.password);
				//user.no_of_bookings=0;
				printf("\n1.CUSTOMER\n2.ADMIN\n3.AGENT\nCHOOSE USER TYPE:");
				scanf("%d",&ch);
				while(ch<1 || ch>3)
				{
					printf("\nINVALID CHOICE!PLEASE RETRY ");
					printf("\n1.CUSTOMER \n2.ADMIN \n3.AGENT \nCHOOSE USER TYPE:");
					scanf("%d",&ch);
				}	
				sr.user.userType=ch;
				sr.actionType=MODIFY_USER;
				//printf("\nVALUE SENding FROM CLIENT");
				send(clientSock,&sr,sizeof(sr),0);
				//perror("SEND");
				read(clientSock,&res,sizeof(res));
				if(res.actionStatus==SUCCESS)
				{
					printf("\nUSER DETAILS MODIFIED SUCCESSFULLY:)");
				}
				else if(res.actionStatus==DOESNOTEXIST)
				{
					printf("\n USER DOESNOT EXIST:(");
				}
				else
				{
					printf("\nMODIFY USER DETAILS OPERATION FAILED:(");
				}
			}
			else if(res.actionStatus==DOESNOTEXIST)
			{
				printf("\n USER ID DOES NOT EXIST!");
			}
			else
			{
				printf("\n MODIFY OPERATION FAILED:(");
			}
			
			
			break;
		case 8:
			printf("\nENTER USER ID TO BE SEARCHED:");
			scanf("%d",&sr.user.userId);
			sr.actionType=SEARCH_USER;
			//printf("\nVALUE Sending FROM CLIENT");
			send(clientSock,&sr,sizeof(sr),0);
			//perror("SEND");
			read(clientSock,&res,sizeof(res));
			if(res.actionStatus==SUCCESS)
			{
				printf("\n:::::SEARCH RESULT::::");
				printf("\nUSER ID    \t USERNAME    \t ROLE");
				if(res.user.userType==CUSTOMER)
					strcpy(userType,"CUSTOMER");
				else if(res.user.userType==ADMIN)
					strcpy(userType,"ADMIN");
				else
					strcpy(userType,"AGENT");
			
				printf("\n%d\t     \t%s\t      \t%s",res.user.userId,res.user.userName,userType);
			}
			else if(res.actionStatus==DOESNOTEXIST)
			{
				printf("\n USER DOES NOT EXIST:(");
			}
			else
			{
				printf("\nUSER SEARCHING FAILED:(");
			}
			break;
		
		/*
		case 9:
			printf("\n--------------------TRAIN DETAILS--------------------------------");
			printf("\nTRAIN NO\t TOTAL SEATS \t AVAILABLE SEATS");
			fd=open("TRAINS.txt",O_RDONLY);
			lseek(fd,0,SEEK_SET);
			while(read(fd,&train,sizeof(train))>0)
			{
				printf("\n%d\t     \t%d\t      \t%d",train.train_no,train.total_seats,train.available_seats);
			}
			close(fd);
			break;
		case 10:
			printf("\n--------------------USER DETAILS--------------------------------");
			printf("\nUSER ID \t USERNAME \tROLE");
			fd=open("ACCOUNTS.txt",O_RDONLY);
			lseek(fd,0,SEEK_SET);
			while(read(fd,&user,sizeof(user))>0)
			{
				printf("\n%d\t  \t%s\t  \t%d",user.userId,user.userName,user.userType);
			}
			close(fd);
			break;
				*/
		case 9:
			sr.actionType=SIGNOUT;
			send(clientSock,&sr,sizeof(sr),0);
			read(clientSock,&res,sizeof(res));
			break;
			
			
	 }
	 if(choice==9)
	 {
	 	printf("\nSIGN OUT SUCCESSFUL!\n");
	 	exit(1);
	 }
	 
      }
}

void customerMenu(int clientSock,int userId)
{
	while(1)
	{

		int choice;
		struct trainDetails train;
		struct accounts user;
		struct bookingResponse bookings;
		printf("\n****************************************************************************");
		printf("\n1.BOOK TRICKET\n2.VIEW PREVIOUS BOOKINGS\n3.UPDATE BOOKING\n 4.CANCEL BOOKING\n5.SIGNOUT");
		printf("\n****************************************************************************");
		printf("\nEnter Your Choice : ");
		
		scanf("%d",&choice);
		/*while(choice<0 && choice >11 && !isNumeric(choice+'0'))
		{
			printf("\nInvalid Entry! Try ");
			printf("\nEnter Your Choice : ");
			scanf("%d",&choice);
		}*/
		int ch;
		int fd;
		int pos;
		struct bookingDetails book;
		struct serviceRequest sr;
		struct serverResponse res;
		switch(choice)
		{
			case 1: 
				printf("\nENTER TRAIN NUMBER:");
				scanf("%d",&sr.bk.train_no);
				printf("\nENTER NO OF SEATS:");
				scanf("%d",&sr.bk.no_of_seats);
				sr.actionType=BOOK_TICKET;
				sr.bk.userId=userId;
				send(clientSock,&sr,sizeof(sr),0);
				//perror("SEND");
				read(clientSock,&res,sizeof(res));
				if(res.actionStatus==SUCCESS)
				{
					printf("\nTICKET BOOKED SUCCESSFULLY, HAPPY JOURNEY:)");
				}
				else if(res.actionStatus==DOESNOTEXIST)
				{
					printf("\n TRAIN DOES NOT EXIST:(");
				}
				else if(res.actionStatus==INSUF_SEATS)
				{
					printf("\nAVAILABLE SEATS ARE LESS THAN REQUIRED:(, MAX AVAILABLE %d",res.train.available_seats);
				}
				else
				{
					printf("\nBOOKING FAILED:(");
				}
				break;
			case 2:
				sr.actionType=VIEW_BOOKING;
				sr.user.userId=userId;
				send(clientSock,&sr,sizeof(sr),0);
				perror("SEND");
				read(clientSock,&res,sizeof(res));
				int cnt=0;
				char bstatus[10];
				if(res.actionStatus==SUCCESS)
				{
					printf("\n************YOUR BOOKINGS******************");
					printf("\nBOOKING_ID\tTRAIN_NO\t# OF SEATS\tBOOKING_STATUS");
					printf("\nNO:%d",res.bks.no_of_bookings);
					while(cnt<res.bks.no_of_bookings)
					{
						
						if(res.bks.bk[cnt].bookingStatus==1)
						{	
							strcpy(bstatus,"CONFIRMED");
						}
						else
							strcpy(bstatus,"CANCELLED");
						printf("\n%d\t\t%d\t\t%d\t\t%s",res.bks.bk[cnt].booking_id,res.bks.bk[cnt].train_no,res.bks.bk[cnt].no_of_seats,bstatus);
						cnt++;
					}
				}
				else if(res.actionStatus==DOESNOTEXIST)
				{
					printf("\n NO BOOKING AVAILABLE:(");
				}
				else
				{
					printf("\nISSUE WHILE FETCHING BOOKINGS :(");
				}
				break;
			case 3:
			        sr.actionType=UPDATE_BOOKING;
			        printf("\nENTER THE BOOKING ID TO MODIFY :");
			        scanf("%d",&pos);
			        sr.bk.booking_id=pos;
			        printf("\nENTER NEW SEAT COUNT :");
			        scanf("%d",&sr.bk.no_of_seats);
			        sr.bk.userId=userId;
			        
			        send(clientSock,&sr,sizeof(sr),0);
			        read(clientSock,&res,sizeof(res));
			        if(res.actionStatus==SUCCESS)
			        {
			        	printf("\nBOOKING MODIFIED SUCCESSFULLY!");
			        	
			        }
			        else if(res.actionStatus==INSUF_SEATS)
			        {
			        	printf("\nAVAILABLE SEATS ARE LESS THAN REQUIRED:(, MAX AVAILABLE %d",res.train.available_seats);
			        }
			        else if(res.actionStatus==DOESNOTEXIST)
			        {
			        	printf("\nBOOKING ID NOT FOUND!");
			        }
			        else if(res.actionStatus==CANCELLED_BK)
			        {
			        	printf("\nCANCELLED BOOKING CANNOT BE UPDATED!");
			        }
			        else
			        {
			        	printf("\nUPDATION FAILED!");
			        }
			        
			        break;
			case 4: 
				sr.actionType=CANCEL_BOOKING;
			        printf("\nENTER THE BOOKING ID TO CANCEL :");
			        scanf("%d",&pos);
			        sr.bk.booking_id=pos;
			        sr.bk.userId=userId;
			        
			        send(clientSock,&sr,sizeof(sr),0);
			        read(clientSock,&res,sizeof(res));
			        if(res.actionStatus==SUCCESS)
			        {
			        	printf("\nBOOKING CANCELLED SUCCESSFULLY!");
			        	
			        }

			        else if(res.actionStatus==DOESNOTEXIST)
			        {
			        	printf("\nBOOKING ID NOT FOUND!");
			        }
			        else
			        {
			        	printf("\nUPDATION FAILED!");
			        }
			        
			        break;
			      /*
			case 6:
				printf("\n--------------------BOOKING DETAILS--------------------------------");
				printf("\nBOOKING ID\tUSERID\tTRAIN No \t NO OF SEATS \tSTATUS");
				fd=open("BOOKINGS.txt",O_RDONLY);
				lseek(fd,0,SEEK_SET);
				while(read(fd,&book,sizeof(book))>0)
				{
					printf("\n%d\t%d\t%d\t%d\t%d",book.booking_id,book.userId,book.train_no,book.no_of_seats,book.bookingStatus);
				}
				close(fd);
				break;*/
			case 5:
				close(clientSock);
				break;
			
			
		}
		if(choice==5)
			exit(0);
	}
}
				
void agentMenu(int clientSock,int userId)
{
	while(1)
	{

		int choice;
		struct trainDetails train;
		struct accounts user;
		struct bookingResponse bookings;
		//printf("\n****************************MENU**********************************************");
		printf("\n1.BOOK TRICKET\n2.VIEW PREVIOUS BOOKINGS\n3.UPDATE BOOKING\n4.CANCEL BOOKING\n5.SIGNOUT");
		printf("\nEnter Your Choice : ");
		
		scanf("%d",&choice);
		printf("\n****************************************************************************");
		/*while(choice<0 && choice >11 && !isNumeric(choice+'0'))
		{
			printf("\nInvalid Entry! Try ");
			printf("\nEnter Your Choice : ");
			scanf("%d",&choice);
		}*/
		int ch;
		int fd;
		int pos;
		struct bookingDetails book;
		struct serviceRequest sr;
		struct serverResponse res;
		switch(choice)
		{
			case 1: 
				printf("\nENTER TRAIN NUMBER:");
				scanf("%d",&sr.bk.train_no);
				printf("\nENTER NO OF SEATS:");
				scanf("%d",&sr.bk.no_of_seats);
				sr.actionType=BOOK_TICKET;
				sr.bk.userId=userId;
				send(clientSock,&sr,sizeof(sr),0);
				perror("SEND");
				read(clientSock,&res,sizeof(res));
				if(res.actionStatus==SUCCESS)
				{
					printf("\nTICKET BOOKED SUCCESSFULLY, HAPPY JOURNEY:)");
				}
				else if(res.actionStatus==DOESNOTEXIST)
				{
					printf("\n TRAIN DOES NOT EXIST:(");
				}
				else if(res.actionStatus==INSUF_SEATS)
				{
					printf("\nAVAILABLE SEATS ARE LESS THAN REQUIRED:(, MAX AVAILABLE %d",res.train.available_seats);
				}
				else
				{
					printf("\nBOOKING FAILED:(");
				}
				break;
			case 2:
				sr.actionType=VIEW_BOOKING;
				sr.user.userId=userId;
				send(clientSock,&sr,sizeof(sr),0);
				perror("SEND");
				read(clientSock,&res,sizeof(res));
				int cnt=0;
				char bstatus[10];
				if(res.actionStatus==SUCCESS)
				{
					printf("\n************YOUR BOOKINGS******************");
					printf("\nBOOKING_ID\tTRAIN_NO\t# OF SEATS\tBOOKING_STATUS");
					printf("\nNO:%d",res.bks.no_of_bookings);
					while(cnt<res.bks.no_of_bookings)
					{
						
						if(res.bks.bk[cnt].bookingStatus==1)
						{	
							strcpy(bstatus,"CONFIRMED");
						}
						else
							strcpy(bstatus,"CANCELLED");
						printf("\n%d\t\t%d\t\t%d\t\t%s",res.bks.bk[cnt].booking_id,res.bks.bk[cnt].train_no,res.bks.bk[cnt].no_of_seats,bstatus);
						cnt++;
					}
				}
				else if(res.actionStatus==DOESNOTEXIST)
				{
					printf("\n NO BOOKING AVAILABLE:(");
				}
				else
				{
					printf("\nISSUE WHILE FETCHING BOOKINGS :(");
				}
				break;
			case 3:
			        sr.actionType=UPDATE_BOOKING;
			        printf("\nENTER THE BOOKING ID TO MODIFY :");
			        scanf("%d",&pos);
			        sr.bk.booking_id=pos;
			        printf("\nENTER NEW SEAT COUNT");
			        scanf("%d",&sr.bk.no_of_seats);
			        sr.bk.userId=userId;
			        
			        send(clientSock,&sr,sizeof(sr),0);
			        read(clientSock,&res,sizeof(res));
			        if(res.actionStatus==SUCCESS)
			        {
			        	printf("\nBOOKING MODIFIED SUCCESSFULLY!");
			        	
			        }
			        else if(res.actionStatus==INSUF_SEATS)
			        {
			        	printf("\nAVAILABLE SEATS ARE LESS THAN REQUIRED:(, MAX AVAILABLE %d",res.train.available_seats);
			        }
			        else if(res.actionStatus==DOESNOTEXIST)
			        {
			        	printf("\nBOOKING ID NOT FOUND!");
			        }
			        else if(res.actionStatus==CANCELLED_BK)
			        {
			        	printf("\nCANCELLED BOOKING CANNOT BE UPDATED!");
			        }
			        else
			        {
			        	printf("\nUPDATION FAILED!");
			        }
			        
			        break;
			        
				
			case 4: 
				sr.actionType=CANCEL_BOOKING;
			        printf("\nENTER THE BOOKING ID TO CANCEL :");
			        scanf("%d",&pos);
			        sr.bk.booking_id=pos;
			        sr.bk.userId=userId;
			        
			        send(clientSock,&sr,sizeof(sr),0);
			        read(clientSock,&res,sizeof(res));
			        if(res.actionStatus==SUCCESS)
			        {
			        	printf("\nBOOKING CANCELLED SUCCESSFULLY!");
			        	
			        }

			        else if(res.actionStatus==DOESNOTEXIST)
			        {
			        	printf("\nBOOKING ID NOT FOUND!");
			        }
			        else
			        {
			        	printf("\nUPDATION FAILED!");
			        }
			        
		        break;
		        /*
			case 6:
				printf("\n--------------------USER DETAILS--------------------------------");
				printf("\nBOOKING ID\tUSERID\tTRAIN No \t NO OF SEATS \tSTATUS");
				fd=open("BOOKINGS.txt",O_RDONLY);
				lseek(fd,0,SEEK_SET);
				while(read(fd,&book,sizeof(book))>0)
				{
					printf("\n%d\t%d\t%d\t%d\t%d",book.booking_id,book.userId,book.train_no,book.no_of_seats,book.bookingStatus);
				}
				close(fd);
				break;
			
			*/
		}
		if(choice==5)
			exit(0);
	}
}
			
