// C program to implement one side of FIFO
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

struct Data {
    char name[20];
    int option;
    char response[512];
};

int main() {
    int fdr, fdw;
    struct Data info;
    char command[30] ="mkfifo ";

    char MENU[80] = "\n \n Select one: \n 1: Social Service \n 2: Professional Practices\n";

    printf("Set your name: \n");
    scanf("%s", info.name);

    strcat(command, info.name);
    // Create the client fifo
    system(command);

    while (1) {
	printf("%s", MENU);
        scanf("%d", &info.option);

        // Open Server FIFO for write only
        fdw = open("server_custom", O_WRONLY);

        // Write the input on FIFO
        // and close it        
        write(fdw, &info, sizeof(info));
        close(fdw);

        // Open Client FIFO for Read only
        fdr = open(info.name, O_RDONLY);
        read(fdr, &info, sizeof(info));
        printf("Server: %s \n", info.response);
        close(fdr);
    }
    return 0;
}
