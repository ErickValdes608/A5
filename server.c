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
    char MENU[80] = "Waiting for client: \n 1: Social Service \n 2: Professional Practices";
    char option[1];

    //Open server connection
    system("mkfifo server");

    int waitForProcessFlag = 0;

    while(1) {
        fdr = open("server", O_RDONLY);
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

        switch(info.option) {
        case 1: ;
            //int pid;
            int fd[2];
            //pid = fork();
            switch(fork()) {
                case 0 :
                    close(fd[0]);
                    printf("Subprocess pid: %i \n", getpid());

                    //Read and send Social Service information from file
                    int fdss = open("ss.txt", O_RDONLY);

                    if(fdss == -1) {
                        perror("Failed to open file\n");
                    } else {
                        read(fdss, content, sizeof(content));
                        printf("File content: %s\n", content);
                        write(fd[1], content, sizeof(content));
                        close(fd[1]);
                        close(fdss);
                    }
                break;
                default:
                    //printf("Subprocess pid: %i \n", getpid());
                    close(fd[1]);
                    read(fd[0], content, sizeof(content));
                    close(fd[0]);

                    printf("Fork content: %s\n", content);
                    strcpy(info.response, content);
                    
                    // Now open in write mode the user pipe and write
                    // the string taken from the fork.
                    fdw = open(info.name, O_WRONLY);

                    write(fdw, &info, sizeof(info));
                    close(fdw);
                    
                    waitForProcessFlag = 0;
                break;
            }
            break;
        case 2: ;
            int mq;
            key_t key = ftok("pp", 123);
            mq = msgget( key, IPC_CREAT | 0600 | IPC_NOWAIT );
            int serverId = 123;
            int processId = 321;
            
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