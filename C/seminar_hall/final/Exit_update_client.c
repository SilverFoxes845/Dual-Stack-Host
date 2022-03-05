#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>

//TO use fgets after scanf
void
scan (int *p)
{
  scanf ("%d", p);
  while ((getchar ()) != '\n');
  return;
}

void
Printf (const char *msg)
{
  printf
    ("\n\n\n-------------------------------------------%s---------------------------------------\n\n\n\n",
     msg);
}

typedef struct exchange
{

  int type;			/*0 - booking 
				   1 - cancelling by date
				   2 - cancelling by booking id
				   3-Display all 
				   4-exit */


  int hall;			//Old Seminar hall-0(take H1)
  //New Seminar hall 1(take h2)
  int month_type;		//current month-0
  //next month-1
  int event_day;		//month.date1[event_day-1]

  int slot_no;			//month.date1[event_day-1].slots[slot_no]

  char id[15];			//booking id for cancellation
} exchange;

typedef struct slot
{
  int booking_status;
  char name[50];
  char contact[11];
  char booking_id[17];
} slot;


typedef struct date_status
{

  unsigned char status[31];
  int month;
  int year;
} date_status;


void
Char_to_Binary (unsigned char s)
{
  int binary[8];
  for (int n = 0; n < 8; n++)
    binary[7 - n] = (s >> n) & 1;
  /* print result */
  for (int n = 1; n < 8; n++)
    printf ("%d\t", binary[n]);
  printf ("\n");
}

void
display_status (date_status status)
{
  printf
    ("Date\t     slot-1  slot-2  slot-3  slot-4  slot-5  slot-6  slot-7\n");

  for (int i = 0; i < 31; i++)
    if (status.status[i] != 0)
      {
	printf ("\n\n%d-%02d-%04d\t", i + 1, status.month, status.year);
	Char_to_Binary (status.status[i]);
      }
}


void
disp ()
{

  printf ("\t\tClient runnnig on IPv4 ONLY  Host\n\n");
}

void
Display_slot (slot info)
{

  printf ("\n\tBooking Details\n\n");
  printf ("\tName : %s\n", info.name);
  printf ("\tContact Number : %s\n\n", info.contact);
  int day, month, year, hall, slot;
  sscanf (info.booking_id, "%02d-%02d-%04d-%d-%d", &day, &month, &year, &hall,
	  &slot);
  printf ("\tDate - %02d/%02d/%d\n\n", day, month, year);
  if (hall == 1)
    printf ("\tSeminar Hall : Old Seminar Hall\n\n");
  if (hall == 2)
    printf ("\tSeminar Hall : New Seminar Hall\n\n");
  char time[12];
  switch (slot)
    {
    case 1:
      strcpy (time, "9AM - 10AM");
      break;
    case 2:
      strcpy (time, "10AM - 11AM");
      break;
    case 3:
      strcpy (time, "11AM - 12PM");
      break;
    case 4:
      strcpy (time, "12PM - 1PM");
      break;
    case 5:
      strcpy (time, "1PM - 2PM");
      break;
    case 6:
      strcpy (time, "2PM - 3PM");
      break;
    case 7:
      strcpy (time, "3PM - 4PM");
      break;
    }
  printf ("\tSlot Time :  %s\n\n", time);
  printf ("\tBooking Id : %s\n\n", info.booking_id);
  return;
}

char *
Time ()
{
  time_t t;
  time (&t);
  return ctime (&t);
}

void
error (const char *msg)
{
  perror (msg);			//inbuilt function that outputs with stderr with error number
  exit (1);
}

int
main (int argc, char *argv[])
{				//to take the arguments from the command line

//disp();
  int cli_sock, portno, n;
  char recvbuffer[255];
  char recvbuffer2[255];
  int size = 255;
  struct sockaddr_in serv_addr;

  if (argc < 2)
    error ("Usage Filename <Ip Address of Server>");
  portno = 5000;
  cli_sock = socket (AF_INET, SOCK_STREAM, 0);
  if (cli_sock < 0)
    {
      error ("Error opening a socket!!");
    }
  printf ("Socket Created Successfully\n\n");
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons (portno);
  serv_addr.sin_addr.s_addr = inet_addr (argv[1]);
  /* if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
     {
     printf("\n inet_pton error occured\n");
     return 1;
     } */

  if (connect (cli_sock, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) <
      0)
    error ("Connection Failed");
  printf ("Connected to server successfully\n\n");
/*msg Send;
strcpy(Send.name,"Bharath");
strcpy(Send.contact,"123456789");
bzero(recvbuffer,255);
strcpy(recvbuffer,"Hello");
n=send(cli_sock,(void *)&Send,sizeof(Send),0);
if(n<0)error("Error while Communication");
printf("Client  sent to server %s : %s\t %s\n",argv[1],Send.name,Send.contact);*/
//Print time sent
//printf("Sent time= %s\n\n",Time());
int loop;
loop=1;
while(loop){
  exchange req;
  slot con;
  printf
    ("\n\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\tWELCOME TO SEMINAR HALL BOOKING APP\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\tChoose the following\n\n\n\t1.Booking\n\n\n\t2.Cancelling by Booking ID\n\n\n\t3.Display Booked Dates in a Month\n\n\n\t4.Exit\n\n");
  printf
    ("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\nChoice : ");
  scan (&req.type);
//  if (req.type == 5)
  //  {exit (0);close(cli_sock);}
  //while ((getchar()) != '\n');
  switch (req.type -= 1)
    {
    case 0:
      {
	printf
	  ("\n\n\tYou Have chosen For Booking the Hall\n\n\n\tWhich Seminar Hall do You want to Book\n\n\n\t1.Old Seminar Hall\n\n\n\t2.New Seminar Hall\n\n\nChoice? : ");
	scan (&req.hall);
//    while ((getchar()) != '\n');
	req.hall -= 1;
	req.hall ? printf ("\tYou have chosen New Seminar Hall\n") :
	  printf ("\tYou Have chosen Old Seminar hall\n");
	/* printf ("Booking type \n1.Single Day\n2.More than one day\n");
	   scan ( &req.B_day);
	   //    while ((getchar()) != '\n');
	   switch (req.B_day -= 1)
	   {
	   case 0:
	   printf ("One Day Booking\n");break;
	   case 1:
	   printf ("Booking Service not available");
	   exit (1);
	   default:
	   printf ("Invalid Choice");
	   exit (1);
	   } */
	printf ("\n\n\n\t1.Current Month\n\n\n\t2.Next Month\n\n\tChoice? :");
	scan (&req.month_type);
	req.month_type -= 1;
	req.
	  month_type ? printf ("\n\n\tYou have chosen Next Month\n") :
	  printf ("\n\n\tYou have chosen Current Month\n");
	printf ("\n\n\n\tEnter the date you want to book (1-31) : ");
	scan (&req.event_day);
	req.event_day -= 1;
	printf ("\n\n\n\tEnter Slot Number(1-7) : ");
	scan (&req.slot_no);
	req.slot_no -= 1;
	//printf("%d\n%d\n%d\n%d\n%d",req.type,req.hall,req.month_type,req.event_day,req.slot_no);
	//Send to server for checking
	n = send (cli_sock, (void *) &req, sizeof (req), 0);
	if (n < 0)
	  error ("Error sending Details\nTry again");
	//receieve response as slot with booking id and booking status
	n = recv (cli_sock, (slot *) & con, sizeof (con), 0);
	if (n < 0)
	  error ("Error Receieving Status\nTry again");
	//if booking status ==0 take details
	if (con.booking_status == 0)
	  {
	printf("Booking id - %s \n\n",con.booking_id);
	    printf
	      ("\n\n\n\tThe slot is Free\n\n\n\tPlease Provide your Contact Details\n\n\n\tYour Name : ");
	    fgets (con.name, sizeof (con.name), stdin);
	    printf ("\n\n\n\tContact Number : ");
	    fgets (con.contact, sizeof (con.contact), stdin);
	    n = send (cli_sock, (void *) &con, sizeof (con), 0);
	    if (n < 0)
	      error ("Error sending Details\nTry again");
	    Display_slot (con);
	    Printf ("Booking Successfull");

	  }
	else
	  {
	    printf
	      ("\n\n\n\tThe Slot Has already been Booked\nDetails Given\n");
	    Display_slot (con);
	    Printf ("Booking Failed");
	  }
	break;
      }

   /* case 1:
      {
	printf
	  ("\tYou Have chosen For Cancellation of Booking the Hall\n\n\n\tWhich Seminar Hall did You Book\n\n\n\t1.Old Seminar Hall\n\n\n\t2.New Seminar Hall\n\n\tChoice? : ");
	scan (&req.hall);
	req.hall -= 1;
	req.
	  hall ? printf ("\nYou had chosen New seminar Hall\n") :
	  printf ("\nYou Had chosen Old seminar hall\n");
	printf ("\n\n\n\t1.Current Month\n\n\n\t2.Next Month\n\n\tChoice? :");
	scan (&req.month_type);
	req.month_type -= 1;
	req.
	  month_type ? printf ("\n\nYou had chosen Next Month\n") :
	  printf ("\n\nYou had chosen Current Month\n");
	printf ("\n\n\n\tEnter the date you booked (1-31) : ");
	scan (&req.event_day);
	req.event_day -= 1;
	printf ("\n\n\n\tEnter Slot Number(1-7) : ");
	scan (&req.slot_no);
	req.slot_no -= 1;
	//Send when to cancel
	n = send (cli_sock, (void *) &req, sizeof (req), 0);
	if (n < 0)
	  error ("Error sending Details\nTry again");
	char report[255];
	n = recv (cli_sock, &report, sizeof (report), 0);

	Printf (report);
	break;
      }*/
    case 1:
      {
	printf
	  ("\n\n\tYou have chosen Cancellation by Booking ID..\n\n\n\tEnter Booking ID seperated by '-' :");
	fgets (req.id, sizeof (req.id), stdin);
	printf ("\n\n\n\tThe Booking ID is %s", req.id);
	n = send (cli_sock, (void *) &req, sizeof (req), 0);
	//waiting
	if (n < 0)
	  error ("Error sending Details\nTry again");
	char report[255];
	n = recv (cli_sock, &report, sizeof (report), 0);

	Printf (report);
	break;
      }
    case 2:
      {
	printf
	  ("\n\n\tYou Have chosen to Display the details\n\n\n\tWhich Seminar Hall do You want to display\n\n\n\t1.Old Seminar Hall\n\n\n\t2.New Seminar Hall\n\n\tChoice? ");
	scan (&req.hall);
	//    while ((getchar()) != '\n');
	req.hall -= 1;
	req.hall ? printf ("\n\nYou have chosen New Seminar Hall\n") :
	  printf ("\n\nYou Have chosen Old Seminar hall\n");
	printf ("\n\n\n\t1.Current Month\n\n\n\t2.Next Month\n\n\tChoice? :");
	scan (&req.month_type);
	req.month_type -= 1;
	req.
	  month_type ? printf ("\n\nYou have chosen Next Month\n") :
	  printf ("\n\nYou have chosen Current Month\n\n\n");
	n = send (cli_sock, (void *) &req, sizeof (req), 0);
	if (n < 0)
	  error ("Error sending Details\nTry again");
	date_status stat;
	n = recv (cli_sock, &stat, sizeof (stat), 0);
	if (n < 0)
	  error ("Error Recvieving Details\nTry again");
	display_status (stat);
	break;
      }
case 3:
	{
	printf("\n\n\nThank You for Using the Application\n\n\tVisit Again\n\n");
	n = send (cli_sock, (void *) &req, sizeof (req), 0);
        if (n < 0)
          error ("Error sending Details\nTry again");
          close (cli_sock);
          loop=0;
	break;//switch
	//while
	}//case
    default:
      Printf ("Service Not Available");
      exit (1);


    }//switch
}//while
  Printf ("\n\nClient Socket Closed\n\n");
  return 0;

}//return
