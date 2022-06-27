#include "constants.h"

void adminActions(int);
void adminMenu(int);
void customerActions(int);
void customerMenu(int,int);
void agentActions(int);
void agentMenu(int,int);

struct loginDetails
{
	int userType;
	int userId;
	char password[20];
};

struct bookingDetails
{
	int train_no;
	int no_of_seats;
	int userId;
	int bookingStatus;
	int booking_id;
};

struct accounts
{
	int userId;
	int userType;
	char userName[30];
	char password[20];
	//struct bookingDetails booking[ALLOWED_NO_OF_BOOKINGS];
	//int no_of_bookings;
};

struct trainDetails
{
	int train_no;
	int total_seats;
	int available_seats;
};
struct bookingResponse
{
	int no_of_bookings;
	struct bookingDetails bk[ALLOWED_NO_OF_BOOKINGS];
};
struct serviceRequest
{
	int actionType;
	int index;
	struct trainDetails train;
	struct accounts user;
	struct bookingDetails bk;
};
struct serverResponse
{
	int actionStatus;
	struct accounts user;
	struct trainDetails train;
	struct bookingResponse bks;
};



