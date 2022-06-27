#include "headers.h"
#include "structures.h"
#include "constants.h"


void adminActions(int socket)
{
	struct flock accountlock;
	struct flock trlock;
	while(1)
	{
		struct serviceRequest sr;
		struct serverResponse res;
		int fd;
		int rcount=1;
		int pos=0;
		struct trainDetails train;
		struct accounts user;
		read(socket,&sr,sizeof(sr));
		
		accountlock.l_pid=getpid();
		
		trlock.l_pid=getpid();
		switch(sr.actionType)
		{
			case ADD_TRAIN:
				fd=open("TRAINS.txt",O_RDWR);
				res.actionStatus=SUCCESS;
				lseek(fd,0,SEEK_SET);
				while(rcount>0)
				{
					rcount=read(fd,&train,sizeof(train));
					pos++;
					if(train.train_no==sr.train.train_no)
					{
						res.actionStatus=ALREADYEXIST;
						break;
					}
				}
				
				if(res.actionStatus==SUCCESS)
				{
					pos--;
					trlock.l_type=F_WRLCK;
					trlock.l_whence=SEEK_SET;
					trlock.l_start=pos*sizeof(train);
					trlock.l_len=sizeof(train);		
					fcntl(fd,F_SETLKW,&trlock);
					
					int w=write(fd,&sr.train,sizeof(sr.train));
					if(w==-1)
					{
						//perror("WRITE");
						res.actionStatus=FAILED;
					}
					trlock.l_type=F_UNLCK;
					fcntl(fd,F_SETLK,&trlock);
					close(fd);
				}else
				{

					res.actionStatus=ALREADYEXIST;
				}
				//printf("STATUS RES:%d",res.actionStatus);
				
				send(socket,&res,sizeof(res),0);
				break;
			case DELETE_TRAIN:
				fd=open("TRAINS.txt",O_RDWR);
				res.actionStatus=SUCCESS;
				lseek(fd,0,SEEK_SET);
				struct trainDetails dummytrain;
				dummytrain.train_no=-1;
				dummytrain.total_seats=-1;
				dummytrain.available_seats=-1;
				while(rcount>0)
				{
					rcount=read(fd,&train,sizeof(train));
					pos++;
					if(train.train_no==sr.train.train_no)
					{
						res.actionStatus=ALREADYEXIST;
						break;
					}
				}
				
				if(res.actionStatus==ALREADYEXIST)
				{
					pos--;
					trlock.l_type=F_WRLCK;
					trlock.l_whence=SEEK_SET;
					trlock.l_start=pos*sizeof(train);
					trlock.l_len=sizeof(train);		
					fcntl(fd,F_SETLKW,&trlock);
					lseek(fd,pos*sizeof(train),SEEK_SET);
					int w=write(fd,&dummytrain,sizeof(sr.train));
					if(w==-1)
					{
						perror("WRITE");
						res.actionStatus=FAILED;
					}
					res.actionStatus=SUCCESS;
					trlock.l_type=F_UNLCK;
					fcntl(fd,F_SETLK,&trlock);
					close(fd);
				}else
				{

					res.actionStatus=DOESNOTEXIST;
				}
				//printf("STATUS RES:%d",res.actionStatus);
				
				send(socket,&res,sizeof(res),0);
				break;
			case SEARCH_TRAIN:
				fd=open("TRAINS.txt",O_RDWR);
				res.actionStatus=DOESNOTEXIST;
				trlock.l_type=F_RDLCK;
				trlock.l_whence=SEEK_SET;
				trlock.l_start=0;
				trlock.l_len=0;		
				fcntl(fd,F_SETLKW,&trlock);
				lseek(fd,0,SEEK_SET);
				while(rcount>0)
				{
					pos++;
					rcount=read(fd,&train,sizeof(train));
					if(train.train_no==sr.train.train_no)
					{
						res.actionStatus=ALREADYEXIST;
						break;
					}
				}
				
	
				if(res.actionStatus==ALREADYEXIST)
				{
					pos--;
					lseek(fd,pos*sizeof(train),SEEK_SET);
					int w=read(fd,&res.train,sizeof(res.train));
					if(w==-1)
					{
						perror("READ");
						res.actionStatus=FAILED;
					}
					res.train=train;
					res.actionStatus=SUCCESS;
					close(fd);
				}else
				{

					res.actionStatus=DOESNOTEXIST;
				}
				trlock.l_type=F_UNLCK;
				fcntl(fd,F_SETLK,&trlock);
				//printf("STATUS RES:%d",res.actionStatus);
				send(socket,&res,sizeof(res),0);
				break;
			case MODIFY_TRAIN:
				fd=open("TRAINS.txt",O_RDWR);
				//printf("\nFIle Opened!");
				res.actionStatus=DOESNOTEXIST;
				lseek(fd,0,SEEK_SET);
				while(rcount>0)
				{
					rcount=read(fd,&train,sizeof(train));
					pos++;
					if(train.train_no==sr.train.train_no)
					{
						res.actionStatus=ALREADYEXIST;

						break;
					}
				}
				send(socket,&res,sizeof(res),0);
				if(res.actionStatus==ALREADYEXIST)
				{
					pos--;
					read(socket,&sr,sizeof(sr));
					//printf("READING WHEN FOUND");
					trlock.l_type=F_WRLCK;
					trlock.l_whence=SEEK_SET;
					trlock.l_start=pos*sizeof(train);
					trlock.l_len=sizeof(train);		
					fcntl(fd,F_SETLKW,&trlock);
					lseek(fd,pos*sizeof(sr.train),SEEK_SET);
					int w=write(fd,&sr.train,sizeof(sr.train));
					if(w==-1)
					{
						perror("WRITE");
						res.actionStatus=FAILED;
					}
					res.actionStatus=SUCCESS;
					trlock.l_type=F_UNLCK;
					fcntl(fd,F_SETLK,&trlock);
					close(fd);
				}else
				{

					res.actionStatus=DOESNOTEXIST;
				}
				//printf("STATUS RES:%d",res.actionStatus);
				
				send(socket,&res,sizeof(res),0);
				break;
			case ADD_USER:
				fd=open("ACCOUNTS.txt",O_RDWR);
				res.actionStatus=SUCCESS;
				lseek(fd,0,SEEK_SET);
				while(rcount>0)
				{
					rcount=read(fd,&user,sizeof(user));
					pos++;
					if(user.userId==sr.user.userId)
					{
						res.actionStatus=ALREADYEXIST;
						break;
					}
				}
	
				if(res.actionStatus==SUCCESS)
				{
					pos--;
					accountlock.l_type=F_WRLCK;
					accountlock.l_whence=SEEK_SET;
					accountlock.l_start=pos*sizeof(user);
					accountlock.l_len=sizeof(user);		
					fcntl(fd,F_SETLKW,&accountlock);
					int w=write(fd,&sr.user,sizeof(sr.user));
					if(w==-1)
					{
						perror("WRITE");
						res.actionStatus=FAILED;
					}
					accountlock.l_type=F_UNLCK;
					fcntl(fd,F_SETLK,&accountlock);
					close(fd);
				}else
				{

					res.actionStatus=ALREADYEXIST;
				}
				//printf("STATUS RES:%d",res.actionStatus);
				
				send(socket,&res,sizeof(res),0);
				close(fd);
				break;
			case DELETE_USER:
				fd=open("ACCOUNTS.txt",O_RDWR);
				res.actionStatus=SUCCESS;
				lseek(fd,0,SEEK_SET);
				struct accounts dummyuser;
				dummyuser.userId=-1;
				strcpy(dummyuser.userName,"");
				strcpy(dummyuser.password,"");
				dummyuser.userType=-1;
				while(rcount>0)
				{
					rcount=read(fd,&user,sizeof(user));
					pos++;
					if(user.userId==sr.user.userId)
					{
						res.actionStatus=ALREADYEXIST;
						break;
					}
				}
	
				if(res.actionStatus==ALREADYEXIST)
				{
					pos--;
					accountlock.l_type=F_WRLCK;
					accountlock.l_whence=SEEK_SET;
					accountlock.l_start=pos*sizeof(user);
					accountlock.l_len=sizeof(user);		
					fcntl(fd,F_SETLKW,&accountlock);
					lseek(fd,pos*sizeof(user),SEEK_SET);
					int w=write(fd,&dummyuser,sizeof(dummyuser));
					if(w==-1)
					{
						perror("WRITE");
						res.actionStatus=FAILED;
					}
					accountlock.l_type=F_UNLCK;
					fcntl(fd,F_SETLK,&accountlock);
					res.actionStatus=SUCCESS;
					close(fd);
				}else
				{

					res.actionStatus=DOESNOTEXIST;
				}
				//printf("STATUS RES:%d",res.actionStatus);
				
				send(socket,&res,sizeof(res),0);
				close(fd);
				break;
			case SEARCH_USER:
				fd=open("ACCOUNTS.txt",O_RDWR);
				res.actionStatus=DOESNOTEXIST;
				accountlock.l_type=F_RDLCK;
				accountlock.l_whence=SEEK_SET;
				accountlock.l_start=0;
				accountlock.l_len=0;		
				fcntl(fd,F_SETLKW,&accountlock);
				lseek(fd,0,SEEK_SET);
				while(rcount>0)
				{
					
					rcount=read(fd,&user,sizeof(user));
					pos++;
					//printf("\n%d,%d",user.userId,sr.user.userId);
					if(user.userId==sr.user.userId)
					{
						res.actionStatus=SUCCESS;
						break;
					}
				}
	
				if(res.actionStatus==SUCCESS)
				{
					pos--;
					lseek(fd,pos*sizeof(user),SEEK_SET);
					int w=read(fd,&user,sizeof(user));
					if(w==-1)
					{
						perror("READ");
						res.actionStatus=FAILED;
					}
					
					res.user=user;
					close(fd);
				}else
				{

					res.actionStatus=DOESNOTEXIST;
				}
				accountlock.l_type=F_UNLCK;
				fcntl(fd,F_SETLK,&accountlock);
				//printf("STATUS RES:%d",res.actionStatus);
				send(socket,&res,sizeof(res),0);
				close(fd);
				break;
			case MODIFY_USER:
				fd=open("ACCOUNTS.txt",O_RDWR);
				res.actionStatus=DOESNOTEXIST;
				lseek(fd,0,SEEK_SET);
				while(rcount>0)
				{
					rcount=read(fd,&user,sizeof(user));
					pos++;
					//printf("\n%d,%d",user.userId,sr.user.userId);
					if(user.userId==sr.user.userId)
					{
						res.actionStatus=ALREADYEXIST;
						break;
					}
				}
				send(socket,&res,sizeof(res),0);
				if(res.actionStatus==ALREADYEXIST)
				{
					pos--;
					accountlock.l_type=F_WRLCK;
					accountlock.l_whence=SEEK_SET;
					accountlock.l_start=pos*sizeof(accountlock);
					accountlock.l_len=sizeof(accountlock);		
					fcntl(fd,F_SETLKW,&accountlock);
					read(socket,&sr,sizeof(sr));
					lseek(fd,pos*sizeof(sr.user),SEEK_SET);
					int w=write(fd,&sr.user,sizeof(sr.user));
					if(w==-1)
					{
						perror("WRITE");
						res.actionStatus=FAILED;
					}
					res.actionStatus=SUCCESS;
					accountlock.l_type=F_UNLCK;
					fcntl(fd,F_SETLK,&accountlock);
					close(fd);
				}else
				{

					res.actionStatus=DOESNOTEXIST;
				}
				//printf("STATUS RES:%d",res.actionStatus);
				
				send(socket,&res,sizeof(res),0);
				close(fd);
				break;
			case SIGNOUT:
				close(fd);
				close(socket);
				return;
			break;
		}
	}
				
				
					
}

void customerActions(int socket)
{
	struct flock bklock;
	struct flock trlock;
	while(1)
	{
		struct serviceRequest sr;
		struct serverResponse res;
		int fd,bfd,tfd,bookfd,trainfd;
		int rcount=1;
		int pos=0,bpos=0,tpos=0;
		int seatCnt=0;
		struct trainDetails train;
		struct accounts user;
		struct bookingDetails bk,newbk;
		struct bookingResponse bookings;
		read(socket,&sr,sizeof(sr));
		bklock.l_pid=getpid();
		
		trlock.l_pid=getpid();
		switch(sr.actionType)
		{
			case BOOK_TICKET:
			                tfd=open("TRAINS.txt",O_RDWR,0666);
					perror("OPEN TRAINS");
					lseek(tfd,0,SEEK_SET);
					rcount=1;
					int c=1;
					res.actionStatus=DOESNOTEXIST;
					trlock.l_type=F_WRLCK;
					trlock.l_whence=SEEK_SET;
					trlock.l_start=0;
					trlock.l_len=0;		
					fcntl(tfd,F_SETLKW,&trlock);
					rcount=read(tfd,&train,sizeof(train));
					while(rcount>0)
					{
						
						tpos++;
						printf("\n%d,%d",sr.bk.train_no,train.train_no);
						if(sr.bk.train_no==train.train_no)
						{
							res.actionStatus=ALREADYEXIST;
							if(train.available_seats>=sr.bk.no_of_seats)
							{
								//printf("\nSEATS FOUND");
								res.actionStatus=SUCCESS;
								train.available_seats=train.available_seats-sr.bk.no_of_seats;
								lseek(tfd,(tpos-1)*sizeof(train),SEEK_SET);
								c=write(tfd,&train,sizeof(train));
								if(c==-1)
								{
									perror("WRITE MODIFIED SEAT COUNT");
									res.actionStatus=FAILED;
								}
										
								break;
							}
							else
							{
								res.actionStatus=INSUF_SEATS;
								res.train.available_seats=train.available_seats;
								break;
							}
						}
						rcount=read(tfd,&train,sizeof(train));
					}
					trlock.l_type=F_UNLCK;
					fcntl(tfd,F_SETLK,&trlock);
					rcount=1;
					if(res.actionStatus==SUCCESS)
					{
						bfd=open("BOOKINGS.txt",O_RDWR|O_CREAT,0666);
						perror("OPEN BOOOKING");
						rcount=read(bfd,&bk,sizeof(bk));
						bpos++;
						while(rcount>0)
						{
							bpos++;
							rcount=read(bfd,&bk,sizeof(bk));
							//printf("\n%d,%d",sr.bk.train_no,train.train_no);
						}
						
						
						//res.actionStatus=DOESNOTEXIST;
						bklock.l_type=F_WRLCK;
						bklock.l_whence=SEEK_SET;
						bklock.l_start=(bpos-1)*sizeof(bk);
						bklock.l_len=sizeof(bk);		
						fcntl(bfd,F_SETLKW,&bklock);
						lseek(bfd,(bpos-1)*sizeof(bk),SEEK_SET);
						bk.userId=sr.bk.userId;
						bk.train_no=sr.bk.train_no;
						bk.no_of_seats=sr.bk.no_of_seats;
						bk.bookingStatus=CONFIRMED;
						bk.booking_id=bpos;
						write(bfd,&bk,sizeof(bk));
						//perror("WRITE");
						bklock.l_type=F_UNLCK;
						fcntl(bfd,F_SETLK,&bklock);
					}
						
					//printf("STATUS RES:%d",res.actionStatus);
					close(bfd);
					close(tfd);
					send(socket,&res,sizeof(res),0);
					break;
					
			case VIEW_BOOKING:
					bfd=open("BOOKINGS.txt",O_RDWR);
					//perror("OPEN BOOKINGS");
					res.actionStatus=DOESNOTEXIST;
					bklock.l_type=F_RDLCK;
					bklock.l_whence=SEEK_SET;
					bklock.l_start=0;
					bklock.l_len=0;		
					fcntl(bfd,F_SETLKW,&bklock);
					lseek(bfd,0,SEEK_SET);
					rcount=1;
					int cnt=0;
					
					
					while(rcount>0)
					{
						bpos++;
						rcount=read(bfd,&newbk,sizeof(newbk));
						if(newbk.userId==sr.user.userId && rcount!=0)
						{
							res.bks.bk[cnt]=newbk;
							//lseek(bfd,(bpos-1)*sizeof(bk),SEEK_SET);
							//read(bfd,&newbk,sizeof(newbk));
							//res.bks.bk[cnt].booking_id=bpos;
							res.actionStatus=SUCCESS;
							cnt++;
						}
						
						//printf("\n%d,%d",sr.bk.train_no,train.train_no);
						
					}
					//printf("\nCOUNT VALUE :%d",cnt);
					res.bks.no_of_bookings=cnt;
					bklock.l_type=F_UNLCK;
					fcntl(bfd,F_SETLK,&bklock);
					//printf("STATUS RES:%d",res.actionStatus);
					close(bfd);
					//perror("CLOSE BFD");
					//close(tfd);
					send(socket,&res,sizeof(res),0);
					break;
			case UPDATE_BOOKING:
					bfd=open("BOOKINGS.txt",O_RDWR);
					perror("OPEN B");
					//bookfd=open("BOOKINGS.txt",O_RDWR);
					tfd=open("TRAINS.txt",O_RDWR);
					perror("OPEN T");
					res.actionStatus=DOESNOTEXIST;
					bklock.l_type=F_RDLCK;
					bklock.l_whence=SEEK_SET;
					bklock.l_start=0;
					bklock.l_len=0;		
					fcntl(bfd,F_SETLKW,&bklock);
					while(rcount>0)
					{
						bpos++;
						rcount=read(bfd,&bk,sizeof(bk));
						printf("\n%d,%d,%d",bk.train_no,bk.booking_id,sr.bk.booking_id);
						if(bk.booking_id==sr.bk.booking_id && sr.bk.userId==bk.userId)
						{
							if(bk.bookingStatus==CANCELLED)
							{
							res.actionStatus=CANCELLED_BK;
							break;
							}
						
							//printf("\nMATCHED AT %d",bpos);
							res.actionStatus=ALREADYEXIST;
							seatCnt=bk.no_of_seats;
							break;
						}
						//printf("\n%d,%d",sr.bk.train_no,train.train_no);
						
					}
					bklock.l_type=F_UNLCK;
					fcntl(bfd,F_SETLK,&bklock);
					rcount=1;
					trlock.l_type=F_RDLCK;
					trlock.l_whence=SEEK_SET;
					trlock.l_start=0;
					trlock.l_len=0;		
					fcntl(tfd,F_SETLKW,&trlock);
					if(res.actionStatus==ALREADYEXIST)
					{
						while(rcount>0)
						{
							tpos++;
							rcount=read(tfd,&train,sizeof(train));
							printf("\n%d,%d",train.train_no,sr.bk.train_no);
							if(train.train_no==sr.bk.train_no && rcount!=0) 
							{
								//printf("\n%d,%d",train.train_no,sr.bk.train_no);
								if(train.available_seats<sr.bk.no_of_seats)
								{
									res.actionStatus=INSUF_SEATS;
									res.train.available_seats=train.available_seats;
								}
								//printf("\nMATCHED TRAIN AT %d",tpos);
								break;
							}
							//printf("\n%d,%d",sr.bk.train_no,train.train_no);
							
						}
					}
					trlock.l_type=F_UNLCK;
					fcntl(tfd,F_SETLK,&trlock);
					if(res.actionStatus==ALREADYEXIST)
					{
						//printf("\nTRAIN FOUND AT POS %d",tpos);
						trlock.l_type=F_WRLCK;
						trlock.l_whence=SEEK_SET;
						trlock.l_start=(tpos-1)*sizeof(train);
						trlock.l_len=sizeof(train);		
						fcntl(tfd,F_SETLKW,&trlock);
						lseek(tfd,(tpos-1)*sizeof(train),SEEK_CUR);
						if(sr.bk.no_of_seats>seatCnt)
						{
							train.available_seats=train.available_seats-(sr.bk.no_of_seats-seatCnt);
						}
						else
						{
							train.available_seats=train.available_seats+(seatCnt-sr.bk.no_of_seats);
						}
						write(tfd,&train,sizeof(train));
						
						trlock.l_type=F_UNLCK;
						fcntl(tfd,F_SETLK,&trlock);
						
						//perror("TRAIN WRITE");
						bklock.l_type=F_WRLCK;
						bklock.l_whence=SEEK_SET;
						bklock.l_start=(bpos-1)*sizeof(bk);
						bklock.l_len=sizeof(bk);		
						fcntl(bfd,F_SETLKW,&bklock);
						lseek(bfd,(bpos-1)*sizeof(bk),SEEK_SET);
						sr.bk.bookingStatus=CONFIRMED;
						sr.bk.train_no=train.train_no;
						int w=write(bfd,&sr.bk,sizeof(sr.bk));
						bklock.l_type=F_UNLCK;
						fcntl(bfd,F_SETLK,&bklock);
						//perror("BOOKING WRITE");
						if(w!=-1)
						{
							res.actionStatus=SUCCESS;
						}
						else
							res.actionStatus=FAILED;
					}
						close(bfd);
						close(tfd);
						send(socket,&res,sizeof(res),0);
						
					break;
			case CANCEL_BOOKING:
					bookfd=open("BOOKINGS.txt",O_RDWR);
					//perror("OPEN B");
					//bookfd=open("BOOKINGS.txt",O_RDWR);
					trainfd=open("TRAINS.txt",O_RDWR);
					//perror("OPEN T");
					res.actionStatus=DOESNOTEXIST;
					bklock.l_type=F_RDLCK;
					bklock.l_whence=SEEK_SET;
					bklock.l_start=0;
					bklock.l_len=0;		
					fcntl(bookfd,F_SETLKW,&bklock);
					while(rcount>0)
					{
						bpos++;
						rcount=read(bookfd,&bk,sizeof(bk));
						//printf("\n%d,%d",bk.booking_id,sr.bk.booking_id);
						if(bk.booking_id==sr.bk.booking_id)
						{
							//printf("\nMATCHED AT %d",bpos);
							res.actionStatus=ALREADYEXIST;
							seatCnt=bk.no_of_seats;
							break;
						}
						//printf("\n%d,%d",sr.bk.train_no,train.train_no);
						
					}
					bklock.l_type=F_UNLCK;
					fcntl(bookfd,F_SETLK,&bklock);
					rcount=1;
					trlock.l_type=F_RDLCK;
					trlock.l_whence=SEEK_SET;
					trlock.l_start=0;
					trlock.l_len=0;		
					fcntl(trainfd,F_SETLKW,&trlock);
					if(res.actionStatus==ALREADYEXIST)
					{
						while(rcount>0)
						{
							tpos++;
							rcount=read(trainfd,&train,sizeof(train));
							//printf("\n%d,%d",train.train_no,sr.bk.train_no);
							if(train.train_no==sr.bk.train_no)
							{
								//printf("\n%d,%d",train.train_no,sr.bk.train_no);
								if(train.available_seats<sr.bk.no_of_seats)
								{
									res.actionStatus=INSUF_SEATS;
									res.train.available_seats=train.available_seats;
								}
								//printf("\nMATCHED TRAIN AT %d",tpos);
								break;
							}
							//printf("\n%d,%d",sr.bk.train_no,train.train_no);
							
						}
					}
					trlock.l_type=F_UNLCK;
					fcntl(trainfd,F_SETLK,&trlock);
					if(res.actionStatus==ALREADYEXIST)
					{
						//printf("\nTRAIN FOUND AT POS %d",tpos);
						trlock.l_type=F_WRLCK;
						trlock.l_whence=SEEK_SET;
						trlock.l_start=(tpos-1)*sizeof(train);
						trlock.l_len=sizeof(train);		
						fcntl(trainfd,F_SETLKW,&trlock);
						lseek(trainfd,(tpos-1)*sizeof(train),SEEK_SET);
	
						train.available_seats=train.available_seats+seatCnt;
						
						write(trainfd,&train,sizeof(train));
						//perror("TRAIN WRITE");
						trlock.l_type=F_UNLCK;
						fcntl(trainfd,F_SETLK,&trlock);
						
						
						bklock.l_type=F_WRLCK;
						bklock.l_whence=SEEK_SET;
						bklock.l_start=(bpos-1)*sizeof(bk);
						bklock.l_len=sizeof(bk);		
						fcntl(bookfd,F_SETLKW,&bklock);
						lseek(bookfd,(bpos-1)*sizeof(bk),SEEK_SET);
						sr.bk.bookingStatus=CANCELLED;
						sr.bk.train_no=train.train_no;
						int w=write(bookfd,&sr.bk,sizeof(sr.bk));
						//perror("BOOKING WRITE");
						bklock.l_type=F_UNLCK;
						fcntl(bookfd,F_SETLK,&bklock);
						if(w!=-1)
						{
							res.actionStatus=SUCCESS;
						}
						else
							res.actionStatus=FAILED;
					}
						close(bookfd);
						close(trainfd);
						send(socket,&res,sizeof(res),0);
						
						break;
			case SIGNOUT:
					close(fd);
					close(socket);
					//return;
					break;
		}
		if(sr.actionType==SIGNOUT)
		{
			close(socket);
			break;
		}
	}
				
				
					
}
void agentActions(int socket)
{
	struct flock bklock;
	struct flock trlock;
	while(1)
	{
		struct serviceRequest sr;
		struct serverResponse res;
		int fd,bfd,tfd,bookfd,trainfd;
		int rcount=1;
		int pos=0,bpos=0,tpos=0;
		int seatCnt=0;
		struct trainDetails train;
		struct accounts user;
		struct bookingDetails bk,newbk;
		struct bookingResponse bookings;
		read(socket,&sr,sizeof(sr));
		bklock.l_pid=getpid();
		
		trlock.l_pid=getpid();
		switch(sr.actionType)
		{
			case BOOK_TICKET:
			                tfd=open("TRAINS.txt",O_RDWR);
					//perror("OPEN TRAINS");
					lseek(tfd,0,SEEK_SET);
					rcount=1;
					int c=1;
					res.actionStatus=DOESNOTEXIST;
					trlock.l_type=F_WRLCK;
					trlock.l_whence=SEEK_SET;
					trlock.l_start=0;
					trlock.l_len=0;		
					fcntl(tfd,F_SETLKW,&trlock);
					rcount=read(tfd,&train,sizeof(train));
					while(rcount>0)
					{
						
						tpos++;
						printf("\n%d,%d",sr.bk.train_no,train.train_no);
						if(sr.bk.train_no==train.train_no)
						{
							res.actionStatus=ALREADYEXIST;
							if(train.available_seats>=sr.bk.no_of_seats)
							{
								//printf("\nSEATS FOUND");
								res.actionStatus=SUCCESS;
								train.available_seats=train.available_seats-sr.bk.no_of_seats;
								lseek(tfd,(tpos-1)*sizeof(train),SEEK_SET);
								c=write(tfd,&train,sizeof(train));
								if(c==-1)
								{
									perror("WRITE MODIFIED SEAT COUNT");
									res.actionStatus=FAILED;
								}
										
								break;
							}
							else
							{
								res.actionStatus=INSUF_SEATS;
								res.train.available_seats=train.available_seats;
								break;
							}
						}
						rcount=read(tfd,&train,sizeof(train));
					}
					trlock.l_type=F_UNLCK;
					fcntl(tfd,F_SETLK,&trlock);
					rcount=1;
					if(res.actionStatus==SUCCESS)
					{
						bfd=open("BOOKINGS.txt",O_RDWR);
						//perror("OPEN BOOOKING");
						rcount=read(bfd,&bk,sizeof(bk));
						bpos++;
						while(rcount>0)
						{
							bpos++;
							rcount=read(bfd,&bk,sizeof(bk));
							//printf("\n%d,%d",sr.bk.train_no,train.train_no);
						}
						
						
						//res.actionStatus=DOESNOTEXIST;
						bklock.l_type=F_WRLCK;
						bklock.l_whence=SEEK_SET;
						bklock.l_start=(bpos-1)*sizeof(bk);
						bklock.l_len=sizeof(bk);		
						fcntl(bfd,F_SETLKW,&bklock);
						//perror("BOOKING WRITE LOCK"); 
						lseek(bfd,(bpos-1)*sizeof(bk),SEEK_SET);
						bk.userId=sr.bk.userId;
						bk.train_no=sr.bk.train_no;
						bk.no_of_seats=sr.bk.no_of_seats;
						bk.bookingStatus=CONFIRMED;
						bk.booking_id=bpos;
						write(bfd,&bk,sizeof(bk));
						//perror("WRITE");
						bklock.l_type=F_UNLCK;
						fcntl(bfd,F_SETLK,&bklock);
						//perror("BOOKING WRITE UNLOCK"); 
					}
						
					//printf("STATUS RES:%d",res.actionStatus);
					close(bfd);
					close(tfd);
					send(socket,&res,sizeof(res),0);
					break;
					
			case VIEW_BOOKING:
					bfd=open("BOOKINGS.txt",O_RDWR);
					//perror("VB OPEN BOOKINGS");
					res.actionStatus=DOESNOTEXIST;
					bklock.l_type=F_RDLCK;
					bklock.l_whence=SEEK_SET;
					bklock.l_start=0;
					bklock.l_len=0;		
					fcntl(bfd,F_SETLKW,&bklock);
					//perror("BOOKING READ LOCK"); 
					lseek(bfd,0,SEEK_SET);
					rcount=1;
					int cnt=0;
					
					
					while(rcount>0)
					{
						bpos++;
						newbk.userId=-1;
						rcount=read(bfd,&newbk,sizeof(newbk));
						if(newbk.userId==sr.user.userId && rcount!=0)
						{
							
							//lseek(bfd,(bpos-1)*sizeof(bk),SEEK_SET);
							//read(bfd,&newbk,sizeof(newbk));
							res.bks.bk[cnt]=newbk;
							//res.bks.bk[cnt].booking_id=bk.booking_id;
							res.actionStatus=SUCCESS;
							cnt++;
						}
						//printf("\n%d,%d",sr.bk.train_no,train.train_no);
						
					}
					//printf("\nAGENT COUNT %d",cnt);
					res.bks.no_of_bookings=cnt;
					bklock.l_type=F_UNLCK;
					fcntl(bfd,F_SETLK,&bklock);
					//perror("BOOKING READ UNLOCK"); 
					
					//printf("STATUS RES:%d",res.actionStatus);
					close(bfd);
					//perror("VB CLOSE BFD");
					//close(tfd);
					send(socket,&res,sizeof(res),0);
					break;
			case UPDATE_BOOKING:
					bookfd=open("BOOKINGS.txt",O_RDWR);
					//perror("OPEN B");
					//bookfd=open("BOOKINGS.txt",O_RDWR);
					trainfd=open("TRAINS.txt",O_RDWR);
					//perror("OPEN T");
					res.actionStatus=DOESNOTEXIST;
					bklock.l_type=F_RDLCK;
					bklock.l_whence=SEEK_SET;
					bklock.l_start=0;
					bklock.l_len=0;		
					fcntl(bookfd,F_SETLKW,&bklock);
					while(rcount>0)
					{
						bpos++;
						rcount=read(bookfd,&bk,sizeof(bk));
						//printf("\n%d,%d",bk.booking_id,sr.bk.booking_id);
						if(bk.booking_id==sr.bk.booking_id && sr.bk.userId==bk.userId)
						{
							if(bk.bookingStatus==CANCELLED)
							{
							res.actionStatus=CANCELLED_BK;
							break;
							}
						
							//printf("\nMATCHED AT %d",bpos);
							res.actionStatus=ALREADYEXIST;
							seatCnt=bk.no_of_seats;
							break;
						}
						//printf("\n%d,%d",sr.bk.train_no,train.train_no);
						
					}
					bklock.l_type=F_UNLCK;
					fcntl(bookfd,F_SETLK,&bklock);
					rcount=1;
					trlock.l_type=F_RDLCK;
					trlock.l_whence=SEEK_SET;
					trlock.l_start=0;
					trlock.l_len=0;		
					fcntl(trainfd,F_SETLKW,&trlock);
					if(res.actionStatus==ALREADYEXIST)
					{
						while(rcount>0)
						{
							tpos++;
							rcount=read(trainfd,&train,sizeof(train));
							//printf("\n%d,%d",train.train_no,sr.bk.train_no);
							if(train.train_no==sr.bk.train_no)
							{
								printf("\n%d,%d",train.train_no,sr.bk.train_no);
								if(train.available_seats<sr.bk.no_of_seats)
								{
									res.actionStatus=INSUF_SEATS;
									res.train.available_seats=train.available_seats;
								}
								//printf("\nMATCHED TRAIN AT %d",tpos);
								break;
							}
							//printf("\n%d,%d",sr.bk.train_no,train.train_no);
							
						}
					}
					trlock.l_type=F_UNLCK;
					fcntl(trainfd,F_SETLK,&trlock);
					if(res.actionStatus==ALREADYEXIST)
					{
						//printf("\nTRAIN FOUND AT POS %d",tpos);
						trlock.l_type=F_WRLCK;
						trlock.l_whence=SEEK_SET;
						trlock.l_start=(tpos-1)*sizeof(train);
						trlock.l_len=sizeof(train);		
						fcntl(trainfd,F_SETLKW,&trlock);
						lseek(trainfd,(tpos-1)*sizeof(train),SEEK_SET);
						if(sr.bk.no_of_seats>seatCnt)
						{
							train.available_seats=train.available_seats-(sr.bk.no_of_seats-seatCnt);
						}
						else
						{
							train.available_seats=train.available_seats+(seatCnt-sr.bk.no_of_seats);
						}
						write(trainfd,&train,sizeof(train));
						
						trlock.l_type=F_UNLCK;
						fcntl(trainfd,F_SETLK,&trlock);
						
						//perror("TRAIN WRITE");
						bklock.l_type=F_WRLCK;
						bklock.l_whence=SEEK_SET;
						bklock.l_start=(bpos-1)*sizeof(bk);
						bklock.l_len=sizeof(bk);		
						fcntl(bookfd,F_SETLKW,&bklock);
						lseek(bookfd,(bpos-1)*sizeof(bk),SEEK_SET);
						sr.bk.bookingStatus=CONFIRMED;
						sr.bk.train_no=train.train_no;
						int w=write(bookfd,&sr.bk,sizeof(sr.bk));
						bklock.l_type=F_UNLCK;
						fcntl(bookfd,F_SETLK,&bklock);
						//perror("BOOKING WRITE");
						if(w!=-1)
						{
							res.actionStatus=SUCCESS;
						}
						else
							res.actionStatus=FAILED;
					}
						close(bookfd);
						close(trainfd);
						send(socket,&res,sizeof(res),0);
						
					break;
			case CANCEL_BOOKING:
					bookfd=open("BOOKINGS.txt",O_RDWR);
					//perror("OPEN B");
					//bookfd=open("BOOKINGS.txt",O_RDWR);
					trainfd=open("TRAINS.txt",O_RDWR);
					//perror("OPEN T");
					res.actionStatus=DOESNOTEXIST;
					bklock.l_type=F_RDLCK;
					bklock.l_whence=SEEK_SET;
					bklock.l_start=0;
					bklock.l_len=0;		
					fcntl(bookfd,F_SETLKW,&bklock);
					while(rcount>0)
					{
						bpos++;
						rcount=read(bookfd,&bk,sizeof(bk));
						//printf("\n%d,%d",bk.booking_id,sr.bk.booking_id);
						if(bk.booking_id==sr.bk.booking_id)
						{
							//printf("\nMATCHED AT %d",bpos);
							res.actionStatus=ALREADYEXIST;
							seatCnt=bk.no_of_seats;
							break;
						}
						//printf("\n%d,%d",sr.bk.train_no,train.train_no);
						
					}
					bklock.l_type=F_UNLCK;
					fcntl(bookfd,F_SETLK,&bklock);
					rcount=1;
					trlock.l_type=F_RDLCK;
					trlock.l_whence=SEEK_SET;
					trlock.l_start=0;
					trlock.l_len=0;		
					fcntl(trainfd,F_SETLKW,&trlock);
					if(res.actionStatus==ALREADYEXIST)
					{
						while(rcount>0)
						{
							tpos++;
							rcount=read(trainfd,&train,sizeof(train));
							//printf("\n%d,%d",train.train_no,sr.bk.train_no);
							if(train.train_no==sr.bk.train_no)
							{
								printf("\n%d,%d",train.train_no,sr.bk.train_no);
								if(train.available_seats<sr.bk.no_of_seats)
								{
									res.actionStatus=INSUF_SEATS;
									res.train.available_seats=train.available_seats;
								}
								//printf("\nMATCHED TRAIN AT %d",tpos);
								break;
							}
							//printf("\n%d,%d",sr.bk.train_no,train.train_no);
							
						}
					}
					trlock.l_type=F_UNLCK;
					fcntl(trainfd,F_SETLK,&trlock);
					if(res.actionStatus==ALREADYEXIST)
					{
						//printf("\nTRAIN FOUND AT POS %d",tpos);
						trlock.l_type=F_WRLCK;
						trlock.l_whence=SEEK_SET;
						trlock.l_start=(tpos-1)*sizeof(train);
						trlock.l_len=sizeof(train);		
						fcntl(trainfd,F_SETLKW,&trlock);
						lseek(trainfd,(tpos-1)*sizeof(train),SEEK_SET);
	
						train.available_seats=train.available_seats+seatCnt;
						
						write(trainfd,&train,sizeof(train));
						//perror("TRAIN WRITE");
						trlock.l_type=F_UNLCK;
						fcntl(trainfd,F_SETLK,&trlock);
						
						
						bklock.l_type=F_WRLCK;
						bklock.l_whence=SEEK_SET;
						bklock.l_start=(bpos-1)*sizeof(bk);
						bklock.l_len=sizeof(bk);		
						fcntl(bookfd,F_SETLKW,&bklock);
						lseek(bookfd,(bpos-1)*sizeof(bk),SEEK_SET);
						sr.bk.bookingStatus=CANCELLED;
						sr.bk.train_no=train.train_no;
						int w=write(bookfd,&sr.bk,sizeof(sr.bk));
						//perror("BOOKING WRITE");
						bklock.l_type=F_UNLCK;
						fcntl(bookfd,F_SETLK,&bklock);
						if(w!=-1)
						{
							res.actionStatus=SUCCESS;
						}
						else
							res.actionStatus=FAILED;
					}
						close(bookfd);
						close(trainfd);
						send(socket,&res,sizeof(res),0);
						
						break;
			case SIGNOUT:
					close(fd);
					close(socket);
					//return;
					break;
		}
		if(sr.actionType==SIGNOUT)
		{
			close(socket);
			break;
		}
	}
				
				
					
}

