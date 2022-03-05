#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    int fdr, fdw;
    char MENU[80] = "Waiting for: \n 1: Social Service \n 2: Professional Practices";
    char option[1];


    // FIFO file path
    char * myfifo = "/tmp/myfifo";

    // Creating the named file(FIFO)
    // mkfifo(<pathname>,<permission>)
    mkfifo(myfifo, 0666);

    while (1) {
        //Añadir variable de control para que no se impriman cosas no deseadas

        printf("%s\n", MENU);

        // Read response from client
        fdr = open(myfifo, O_RDONLY);
        read(fdr, option, 1);
        // Print the read string and close
        printf("Client sent: %s\n", option);
        close(fdr);

        char str1[80], str2[80];
        char content[1024];

        switch (atoi(option)) {
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