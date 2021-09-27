// C Program for Message Queue (Reader Process)
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include<time.h>
#include <stdbool.h>

// structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_text[100];
} message;

#define MAXLEN 1000
char words[84095][25];
char ltr[1];

void filetoarray() {
        FILE *fp;
        char *line = NULL;
        fp = fopen("dictionary.txt", "r");
        char word[25];

        int count = 0;
        if (fp != NULL) {
                while (fscanf(fp, "%24s", word) > 0) {
                        strcpy(words[count], word);
                        count++;
                }
        }
	fclose(fp);
}


int randomNumber() {
        int randNum;
        srand(time(NULL));
        randNum = (rand()*64*1024+rand())%84095;
        if (randNum < 0) {
                randNum = randNum * -1;
        }
        return randNum;
}
  
int main()
{
	key_t key;
	int msgid;

        // ftok to generate unique key
        key = ftok(getenv("HOME"), 1);
  
  	int counter=3;

/*	filetoarray(); // put words from file into array
        int randNum = randomNumber(); // generate a random number based on number of words
        int length = strlen(words[randNum]); // length of word randomly chosen

	// client guess word
        char gWord[length];
        for (int i = 0; i < length; i++) {
            gWord[i] = '*';
        }
	int gCount = 0;
*/
	while (1) {
        	filetoarray(); // put words from file into array
        	int randNum = randomNumber(); // generate a random number based on number of words
        	int length = strlen(words[randNum]); // length of word randomly chosen

        	// client guess word
        	char gWord[length];
        	for (int i = 0; i < length; i++) {
            		gWord[i] = '*';
        	}
		int gCount = 0;

    		// msgget creates a message queue and returns identifier
    		msgid = msgget(key, 0666 | IPC_CREAT);
  		printf("Key %d Msgid %d\n", key, msgid);

    		// msgrcv to receive message
    		msgrcv(msgid, &message, sizeof(message), 1, 0);
    		// to destroy the message queue
    		msgctl(msgid, IPC_RMID, NULL);
  
    		// display the message
//    		printf("Data Received is : %s \n", message.mesg_text);
		//assign dedicated project #s for communication to client
		int key2 = ftok(getenv("HOME"), 2);
    		int msgid2 = msgget(key2, 0666 | IPC_CREAT);
        	sprintf(message.mesg_text, "%d %d", counter, counter+1);
    		msgsnd(msgid2, &message, sizeof(message), 0);
//    		printf("Data Sent is : %s\n", message.mesg_text);
		counter += 2;

		// for sending a message
                int key3 = ftok(getenv("HOME"), 3);
                int msgid3 = msgget(key3, 0666 | IPC_CREAT);
		
		// for receving a message
        	int key4 = ftok(getenv("HOME"), 4);
        	int msgid4 = msgget(key4, 0666 | IPC_CREAT);

                printf("Word: %s, Length: %d\n", words[randNum], length);

		while (gCount < length) {
 			int msgid4 = msgget(key4, 0666 | IPC_CREAT); // msgget creates a message queue and returns identifier
//			int msgid3 = msgget(key3, 0666 | IPC_CREAT);

			// send message
                	sprintf(message.mesg_text, "%s %s", "(Guess) Enter a letter in word", gWord);
                	msgsnd(msgid3, &message, sizeof(message), 0);		
//			printf("Data : %s \n", message.mesg_text);

			// receive message
			msgrcv(msgid4, &message, sizeof(message), 1, 0);
//			printf("Data Received is : %s \n", message.mesg_text);
			//msgctl(msgid4, IPC_RMID, NULL);
			
			for (int i = 0; i < length; i++) {
				if (words[randNum][i] == message.mesg_text[0]) {
					if (gWord[i] == '*') { // so already revelaed letters not counted again
						gWord[i] = message.mesg_text[0];
						gCount++;
					}
				}					
			}

			//gCount++;						
			if (gCount == length) {
				sprintf(message.mesg_text, "%s", "Completed!");
                        	msgsnd(msgid3, &message, sizeof(message), 0);
				
				sprintf(message.mesg_text, "%s %s", "Word: ", gWord);
                        	msgsnd(msgid3, &message, sizeof(message), 0);
//                        	printf("Data : %s \n", message.mesg_text);
			}		
		}
		msgctl(msgid4, IPC_RMID, NULL);
  	} 
  
    	return 0;
}
