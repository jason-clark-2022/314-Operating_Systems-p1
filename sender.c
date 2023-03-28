/* *************************************************** *
 * sender.c:                                           *
 *                                                     *
 * February 8, 2021                                    *
 *                                                     *
 * compile: "cc sender.c"                              *
 *                                                     *
 * os.cs.siue.edu                                      *
 * *************************************************** */

#include <stdio.h>         // for printf
#include <stdlib.h>        // for sleep()
#include <time.h>          // for srand
#include <sys/types.h>     // for message queue
#include <sys/ipc.h>       // for message queue
#include <sys/msg.h>       // for message queue
#include <unistd.h>        // for sleep()

#define NUM_REPEATS   200  // the number of the messages
#define MSG_key_Q1   6465  // message queue key for Q1
#define MSG_key_Q2   6466  // message queue key for Q2

#define BUFFER_SIZE   256  // max. message size

int main (void)
{
   int  i;                 // loop counter

   int  status_code;       // result status

   int  msqid_Q1;          // message queue ID (Q1)
   int  msqid_Q2;          // message queue ID (Q2)

   key_t msgkey_Q1;        // message-queue key (Q1)
   key_t msgkey_Q2;        // message-queue key (Q2)

   unsigned int       rand_num;  // a 32-bit random number
   float             temp_rand;  // the raw random numbber
   unsigned char eight_bit_num;  // an 8-bit random number

   /* Use current time to shuffle random seed */
   srand(time(0)); 

   /* definition of message ------------------- */
   struct message{
        long mtype;
        unsigned char mtext[BUFFER_SIZE];
   };

   /* instantiate the message buffer -----------*/
   struct message buffer;

   /* set up the message queue key Q1 --------- */
   msgkey_Q1 = MSG_key_Q1; 

   /* create a new message queue -------------- */
   msqid_Q1 = msgget(msgkey_Q1, 0666 | IPC_CREAT);

   /* error check ----------------------------- */
   if (msqid_Q1 <= -1)
   { printf ("your new message queue (Q1) is not created ....\n");  }

   else
   { printf("your new message queue (Q1) is successfully created ....\n"); }

   /* set up the message queue key Q2 --------- */
   msgkey_Q2 = MSG_key_Q2; 

   /* create a new message queue -------------- */
   msqid_Q2 = msgget(msgkey_Q2, 0666 | IPC_CREAT);

   /* error check ----------------------------- */
   if (msqid_Q2 <= -1)
   { printf ("your new message queue (Q2) is not created ....\n");  }

   else
   { printf("your new message queue (Q2) is successfully created ....\n"); }

   /* confirm the start of the two receivers --- */
   printf("start your Middlemen then start the receiver ... press any key when ready ....\n\n");

   /* wait for a key stroke at the keyboard ---- */
   getchar();

   /* take care of "mtype" --------------------- */
   buffer.mtype = 1;  // UNIX standard says, any number

   /* send one eigh-bit number, one at a time  ------------ */
   for (i = 0; i < NUM_REPEATS; i++)
   {
      /* generate an 8-bit random number (0-255) ---------- */
      temp_rand = ((float)rand()/(float)RAND_MAX)*255.0;   
      rand_num = (int)temp_rand;
      eight_bit_num = (unsigned char)rand_num; 
     
      /* se the new eight-bit number to the message buffers ------ */  
      buffer.mtext[0] = eight_bit_num;   // copy the 8-bit number     
      buffer.mtext[1] = '\0';            // null-terminate it
      
      /* send a 8-bit number to Q1 ----------------------- */
      status_code = msgsnd(msqid_Q1, (struct message *)&buffer, sizeof(buffer.mtext), 0);

      /* detect a message transmission error ------------- */
      if (status_code <= -1)
      {
         printf("sending a number to Q1 failed ... \n\a"); 
         
         break;
      }

      /* send a 8-bit number to Q2 ----------------------- */
      status_code = msgsnd(msqid_Q2, (struct message *)&buffer, sizeof(buffer.mtext), 0);

      /* detect a message transmission error ------------- */
      if (status_code <= -1)
      {
         printf("sending a number to Q2 failed ... \n\a"); 
         
         break;
      }
   }

   /* give everyone else enough time to complete their tasks --- */
   sleep(1);

   /* delete the emsage queue ----------------------------- */
   status_code = msgctl(msqid_Q1, IPC_RMID, NULL);

   /* error check ----------------------------------------- */
   if (status_code <= -1)
   { printf("deleting Q1 failed ...\n\a");  } 

   else
   { printf("Q1 message queue has been successfully deleted ...\n"); }

   /* delete the emsage queue ----------------------------- */
   status_code = msgctl(msqid_Q2, IPC_RMID, NULL);

   /* error check ----------------------------------------- */
   if (status_code <= -1)
   { printf("deleting Q2 failed ...\n\a");  } 

   else
   { printf("Q2 message queue has been successfully deleted ...\n"); }
}

/* END OF LINES ============================================================= */