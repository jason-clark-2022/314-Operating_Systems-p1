/* *************************************************** *
 * Jason Clark (442)                                   *
 * Jasocla@siue.edu                                    *
 * m1.c                                                *
 *                                                     *
 * March 1, 2021                                       *
 * compile/create exe: "cc m1.c -o m1.exe"             *
 *                                                     *
 * os.cs.siue.edu                                      *
 * *************************************************** */

#include <stdio.h>         
#include <stdlib.h>        
#include <sys/types.h>     
#include <sys/ipc.h>       
#include <sys/msg.h>       
#include <unistd.h>        

#define MSG_key_Q1   6465  
#define MSG_key_Q3   6467  
#define NUM_REPEATS   200  
#define BUFFER_SIZE   256  

int main (void)
{
    int i;              
    int status_code;     

    int  msqid_Q1;         
    int  msqid_Q3;          

    int counter_even = 0; 	 
	int counter_total = 0; 	 

    key_t msgkey_Q1;       
    key_t msgkey_Q3;     

    struct message{
        long mtype;
        unsigned char mtext[BUFFER_SIZE];
    };

    struct message buffer;

    msgkey_Q1 = MSG_key_Q1; 
    msgkey_Q3 = MSG_key_Q3; 
       
    msqid_Q1 = msgget(msgkey_Q1, 0666);
    if (msqid_Q1 < 0) { printf ("New message queue (Q1) has not been retrieved ....\n");  }

    msqid_Q3 = msgget(msgkey_Q3, 0666 | IPC_CREAT);
    if (msqid_Q3 <= -1)
    { printf ("New message queue (Q3) creation fail ....\n");  }
    else
    { printf("m1 has created the message queue (Q3) to receiver ....\n"); }

    buffer.mtype = 1;  

    while ((msgget(msgkey_Q1, 0666)) >= 0)
    {
        if (msgrcv(msqid_Q1,(struct message *)&buffer,sizeof(buffer.mtext),0,IPC_NOWAIT) == -1) {}
            else {
                counter_total++;
                printf("m1 rcvd.: %i \n", buffer.mtext[0]);
                if(buffer.mtext[0] % 2 == 0)
                {
                    counter_even++;
                    status_code = msgsnd(msqid_Q3, (struct message *)&buffer, sizeof(buffer.mtext), 0);
                    if (status_code <= -1)
                        {
                            printf("sending a number to Q3 failed ... \n\a"); 
                            break;
                        }
                    printf("m1 send.: %i \n", buffer.mtext[0]);
                }
                buffer.mtext[1] = '\0'; 
            }
    }
    sleep(1);
    status_code = msgctl(msqid_Q3, IPC_RMID, NULL);
    if (status_code <= -1) {printf("deleting Q3 failed ...\n\a");} 
    printf("m1 is terminating (recvd: %i, even: %i) ...\n\n", counter_total, counter_even);
}
