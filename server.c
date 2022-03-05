#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

struct Data {
    char name[20];
    int option;
    char response[512];
};

int main() {
    int fdr, fdw;
    char MENU[80] = "Waiting for client: \n 1: Social Service \n 2: Professional Practices";
    char option[1];

    struct Data info;

    //Open server connection
    system("mkfifo server");

    fdr = open("server", O_RDONLY);
    if(fdr == -1) {
        perror("Fatal error");
        exit(1);
    }

    int waitForProcessFlag = 0;

    while (1) {
      if (!waitForProcessFlag)
      {
        waitForProcessFlag = 1;
        //Añadir variable de control para que no se impriman cosas no deseadas

        printf("%s\n", MENU);

        // Read response from client
        read(fdr, &info, sizeof(info));
        // Print the read string and close
        printf("Client sent: %d\n", info.option);
        

        char str1[80], str2[80];
        char content[1024];

        switch (info.option) {
        case 1: ;
            //Read and send Social Service

            int fdss = open("ss.txt", O_RDONLY);

            if(fdss == -1) {
                perror("Failed to open file\n");
            } else {
                read(fdss, content, sizeof(content));

                close(fdss);
            }
            printf("File content: %s\n", content);

            // Now open in write mode and write
            // string taken from file.
            fdw = open(info.name, O_WRONLY);
        //printf(">> Server :  %s" content);
        //    fgets(str2, 80, stdin);
        //printf("\n");
            write(fdw, content, strlen(content)+1);
            close(fdw);
            
            waitForProcessFlag = 0;
            break;
        case 2: ;
            //Read and send Professional Practices
            int fdpp = open("pp.txt", O_RDONLY);

            if(fdpp == -1) {
                perror("Failed to open file\n");
            } else {
                read(fdpp, content, sizeof(content));

                close(fdpp);
            }
            printf("File content: %s\n", content);

            // Now open in write mode and write
            // string taken from file.
            fdw = open(info.name, O_WRONLY);
        //printf(">> Server :  %s" content);
        //    fgets(str2, 80, stdin);
        //printf("\n");
            write(fdw, content, strlen(content)+1);
            close(fdw);
            waitForProcessFlag = 0;
            break;
        default:
            printf("Closing Server\n");
            close(fdr);
            return 0;
            break;
        }
      }
    }
    return 0;
}