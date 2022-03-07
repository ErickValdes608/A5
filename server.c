#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct Data {
    char name[20];
    int option;
    char response[1024];
};

struct MqData {
    long id;
    char message[1024];
};

int main() {
    int fdr, fdw;
    //int pid;
    char MENU[80] = "\n\nWaiting for client: \n 1: Social Service \n 2: Professional Practices";
    char option[1];

    //Open server connection
    system("mkfifo server_custom");

    int waitForProcessFlag = 0;

    while(1) {
        fdr = open("server_custom", O_RDONLY);
        if(fdr == -1) {
            perror("Fatal error");
            exit(1);
        }
        
        if (!waitForProcessFlag)
	      {
		waitForProcessFlag = 1;

		printf("%s\n", MENU);

		struct Data info;
		struct MqData mqInfo;

		// Read response from client
		read(fdr, &info, sizeof(info));
		// Print the read option
		printf("Client %s sent: %d\n", info.name, info.option);
		
		char content[1024];
		int mq;
		int serverId;
		int processId;

		switch(info.option) {
		case 1: ;
		    mq;
		    key_t key_ss = ftok("ss", 1234);
		    mq = msgget( key_ss, IPC_CREAT | 0600 | IPC_NOWAIT );
		    serverId = 1234;
		    processId = 4321;
		    
		    switch(fork()) {
		        case 0 : ;
		            //Read and send Social service information from file
		            int fdpp = open("ss.txt", O_RDONLY);

		            if(fdpp == -1) {
		                perror("Failed to open file\n");
		            } else {
		                read(fdpp, content, sizeof(content));
		                close(fdpp);
		                printf("File content: %s\n", content);
		                mqInfo.id = serverId;
		                strcpy(mqInfo.message, content);
		                msgsnd(mq, &mqInfo, sizeof(mqInfo), 0);
		            }
		        break;
		        default:
		            msgrcv(mq, &mqInfo, sizeof(mqInfo), serverId, 0);
		            printf("MQ content: %s\n", mqInfo.message);
		            strcpy(info.response, mqInfo.message);
		            // Now open in write mode and write
		            // string taken from MQ.
		            fdw = open(info.name, O_WRONLY);

		            write(fdw, &info, sizeof(info));
		            close(fdw);
		            msgctl(mq, IPC_RMID, NULL);
		            waitForProcessFlag = 0;
		        break;
		    }
		    break;
		    
		    
		case 2: ;
		    mq;
		    key_t key = ftok("pp", 123);
		    mq = msgget( key, IPC_CREAT | 0600 | IPC_NOWAIT );
		    serverId = 123;
		    processId = 321;
		    
		    switch(fork()) {
		        case 0 : ;
		            //Read and send Professional Practices information from file
		            int fdpp = open("pp.txt", O_RDONLY);

		            if(fdpp == -1) {
		                perror("Failed to open file\n");
		            } else {
		                read(fdpp, content, sizeof(content));
		                close(fdpp);
		                printf("File content: %s\n", content);
		                mqInfo.id = serverId;
		                strcpy(mqInfo.message, content);
		                msgsnd(mq, &mqInfo, sizeof(mqInfo), 0);
		            }
		        break;
		        default:
		            msgrcv(mq, &mqInfo, sizeof(mqInfo), serverId, 0);
		            printf("MQ content: %s\n", mqInfo.message);
		            strcpy(info.response, mqInfo.message);
		            // Now open in write mode and write
		            // string taken from MQ.
		            fdw = open(info.name, O_WRONLY);

		            write(fdw, &info, sizeof(info));
		            close(fdw);
		            msgctl(mq, IPC_RMID, NULL);
		            waitForProcessFlag = 0;
		        break;
		    }
		    break;
		default:
		    printf("Closing Server\n");
		    return 0;
		    break;
		}
	      }
	    close(fdr);
	    }
    return 0;
}
