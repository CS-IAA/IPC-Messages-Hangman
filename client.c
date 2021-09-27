
// C Program for Message Queue (Writer Process)
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
  
// structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_text[100];
} message;
  
int main(int argc, char *argv[])
{
    	key_t key;
    	int msgid;
  
	// ftok to generate unique key
  	key = ftok(getenv("HOME"), 1);
  
  	//fill the data in struct before sending
    	message.mesg_type = 1;
	message.mesg_text[0] = NULL;
	for(int i=1; i<argc; i++) 
		strcat(message.mesg_text, argv[i]);
  	
    	// msgget creates a message queue
    	// and returns identifier
    	msgid = msgget(key, 0666 | IPC_CREAT);
    	printf("Send Key %d Msgid %d\n", key, msgid);

    	// msgsnd to send message
    	msgsnd(msgid, &message, sizeof(message), 0);
    	// display the message
//    	printf("Data send is : %s\n", message.mesg_text);

    	int key2 = ftok(getenv("HOME"), 2);
    	int msgid2 = msgget(key2, 0666 | IPC_CREAT);
    	printf("Recv Key %d Msgid %d\n", key2, msgid2);

	msgrcv(msgid2, &message, sizeof(message), 1, 0);
//    	printf("Data Received is : %s \n", message.mesg_text);
    	msgctl(msgid2, IPC_RMID, NULL);

    	// recieve message
	int key3 = ftok(getenv("HOME"), 3);
	int msgid3 = msgget(key3, 0666 | IPC_CREAT);
        
	// send message
	int key4 = ftok(getenv("HOME"), 4);
        int msgid4 = msgget(key4, 0666 | IPC_CREAT);
	
	char guess[1];

	while (1) { // while word not guessed
    		// receive message
		int msgid3 = msgget(key3, 0666 | IPC_CREAT);
//		int msgid4 = msgget(key4, 0666 | IPC_CREAT);

		// receive message
		msgrcv(msgid3, &message, sizeof(message), 1, 0);
    		printf("%s \n", message.mesg_text);
		//msgctl(msgid3, IPC_RMID, NULL);
	
		// if word guessed
		if (strcmp(message.mesg_text,"Completed!") == 0) {
			int msgid3 = msgget(key3, 0666 | IPC_CREAT);
			msgrcv(msgid3, &message, sizeof(message), 1, 0);
                	printf("%s \n", message.mesg_text);
			break;
		}

		// send message
		gets(guess);
		sprintf(message.mesg_text, "%s", guess);
		msgsnd(msgid4, &message, sizeof(message), 0);
//		printf("Data send is : %s\n", message.mesg_text);
	}
	msgctl(msgid3, IPC_RMID, NULL);
    	return 0;
}
