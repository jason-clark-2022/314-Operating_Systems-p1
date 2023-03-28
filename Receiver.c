/* *************************************************** *
 * Jason Clark (442)                                   *
 * Jasocla@siue.edu                                    *
 * Receiver.c                                          *
 *                                                     *
 * March 1, 2021                                       *
 * compile/create exe: "cc Receiver.c -o Receiver.exe" *
 *                                                     *
 * os.cs.siue.edu                                      *
 * *************************************************** */

#include <stdio.h>         
#include <sys/types.h>     
#include <sys/ipc.h>      
#include <sys/msg.h>      
#include <stdlib.h>        
#include <unistd.h>     

#define MSG_key_Q3   6467  
#define MSG_key_Q4   6468  
#define NUM_REPEATS   200  
#define BUFFER_SIZE   256  

int main (void)
{
	int  i;                 

	int  msqid_Q3;           
	int  msqid_Q4;          

	int counter_even = 0; 	
	int counter_odd = 0; 	
	
	key_t msgkey_Q3;	
	key_t msgkey_Q4;	 


	
	msgkey_Q3 = MSG_key_Q3;	 
	msgkey_Q4 = MSG_key_Q4;	

	struct message{
		long mtype;
		unsigned char mtext[BUFFER_SIZE];
	};

	struct message buf1;
	struct message buf2;

	msqid_Q3 = msgget(msgkey_Q3, 0666); 
	msqid_Q4 = msgget(msgkey_Q4, 0666); 
	
	if (msqid_Q3 < 0) { printf ("New message queue (Q3) has not been retreived ....\n");  }
	if (msqid_Q4 < 0) { printf ("New message queue (Q4) has not been retreived ....\n");  }
	printf ("the receiver is ready to receive messages from the middleman ....\n");

    sleep(5);

    buf1.mtype = 1;  
	buf2.mtype = 1;

	while ((msgget(msgkey_Q3, 0666)) >= 0 || (msgget(msgkey_Q4, 0666)) >= 0 )
	{
		if (msgrcv(msqid_Q3,(struct message *)&buf1,sizeof(buf1.mtext),0,IPC_NOWAIT) == -1) {}
                else {
                        printf("Received number (Q3) : %i\n", buf1.mtext[0]);
                        counter_even++;
                }
		if (msgrcv(msqid_Q4,(struct message *)&buf2,sizeof(buf2.mtext),0,IPC_NOWAIT) == -1) {}
                else {
                        printf("Received number (Q4) : \t\t\t %i\n", buf2.mtext[0]);
                        counter_odd++;
                }

		buf1.mtext[1] = '\0'; 
		buf2.mtext[1] = '\0';
	}	
	
	printf("receiver is completed ... \n");
	printf("received: %i (even) and %i (odd) numbers from the middlemen ... \n\n", counter_even, counter_odd );
}



























	