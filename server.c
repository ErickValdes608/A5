#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    int fdr, fdw;
    char MENU[80] = "Select one: \n Social Service: 1 \n Professional Practices: 2";
    char option[1];


    // FIFO file path
    char * myfifo = "/tmp/myfifo";

    // Creating the named file(FIFO)
    // mkfifo(<pathname>,<permission>)
    mkfifo(myfifo, 0666);

    while (1) {
        // Sends client the options he has to read the information.
        fdw = open(myfifo, O_WRONLY);
        write(fdw, MENU, strlen(MENU)+1);

        close(fdw);

        char str1[80], str2[80];

        // Read response from client
        fdr = open(myfifo, O_RDONLY);
        read(fdr, option, 1);
        // Print the read string and close
        printf("Client selected: %s\n", option);
        close(fdr);

        switch (atoi(option)) {
        case 1: ;
            //Read and send Social Service

	        //fopen("ss.txt", "r");
            char content[1024];


            // Now open in write mode and write
            // string taken from file.
            fdw = open(myfifo, O_WRONLY);
        //printf(">> Server :  %s" content);
        //    fgets(str2, 80, stdin);
        //printf("\n");
            write(fdw, content, strlen(content)+1);
            close(fdw);
            break;
        case 2:
            //Read and send Professional Practices

            break;
        default:
            printf("Invalid option\n");
            break;
        }
    }
    return 0;
}