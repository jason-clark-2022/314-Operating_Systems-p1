/* *************************************************** *
 * Jason Clark (442)                                   *
 * Jasocla@siue.edu                                    *
 * m2.c                                                *
 *                                                     *
 * March 1, 2021                                       *
 * compile/create exe: "cc m2.c -o m2.exe"             *
 *                                                     *
 * os.cs.siue.edu                                      *
 * *************************************************** */

#include <stdio.h>      
#include <stdlib.h>      
#include <sys/types.h>     
#include <sys/ipc.h>    
#include <sys/msg.h>      
#include <unistd.h>        

#define MSG_key_Q2   6466  
#define MSG_key_Q4   6468  
#define NUM_REPEATS   200  
#define BUFFER_SIZE   256  

int main (void)
{
    int i;              
    int status_code;     

    int  msqid_Q2;          
    int  msqid_Q4;          

    int counter_odd = 0; 	
	int counter_total = 0; 	 

    key_t msgkey_Q2;        
    key_t msgkey_Q4;      

    struct message{
        long mtype;
        unsigned char mtext[BUFFER_SIZE];
    };

    struct message buffer;

    msgkey_Q2 = MSG_key_Q2; 
    msgkey_Q4 = MSG_key_Q4; 
       
    msqid_Q2 = msgget(msgkey_Q2, 0666);
    if (msqid_Q2 < 0) { printf ("New message queue (Q2) has not been retrieved ....\n");  }

    msqid_Q4 = msgget(msgkey_Q4, 0666 | IPC_CREAT);
    if (msqid_Q4 <= -1) { printf ("New message queue (Q4) creation fail ....\n");  }
    else { printf("m2 has created the message queue (Q4) to receiver ....\n"); }

    buffer.mtype = 1;  

    while((msgget(msgkey_Q2, 0666)) >= 0)
    {
		if (msgrcv(msqid_Q2,(struct message *)&buffer,sizeof(buffer.mtext),0,IPC_NOWAIT) == -1) {}
        else {
            counter_total++;
            printf("m1 rcvd.: %i \n", buffer.mtext[0]);
            if(buffer.mtext[0] % 2 == 1)
            {
                counter_odd++;
                status_code = msgsnd(msqid_Q4, (struct message *)&buffer, sizeof(buffer.mtext), 0);
                if (status_code <= -1)
                    {
                        printf("sending a number to Q4 failed ... \n\a"); 
                        break;
                    }
                printf("m1 send.: %i \n", buffer.mtext[0]);
            }
            buffer.mtext[1] = '\0'; 
        }
    }
    sleep(1);
    status_code = msgctl(msqid_Q4, IPC_RMID, NULL);
    if (status_code <= -1) { printf("deleting Q4 failed ...\n\a");  } 
    printf("m2 is terminating (recvd: %i, odd: %i) ... \n\n", counter_total, counter_odd);
}
