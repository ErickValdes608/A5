// C program to implement one side of FIFO
// This side writes first, then reads
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    int fd;
    char MENU[80] = "Select one: \n 1: Social Service \n 2: Professional Practices";

    // FIFO file path
    char * myfifo = "/tmp/myfifo";

    // Creating the named file(FIFO)
    // mkfifo(<pathname>, <permission>)
    mkfifo(myfifo, 0666);

    char arr1[80], resp[2];
    while (1) {

        // Open FIFO for write only
        fd = open(myfifo, O_WRONLY);

        // Take an input arr2ing from user.
        // 80 is maximum length
	printf(">> Client 1:  ");
        fgets(resp, 2, stdin);
	printf("\n");

        // Write the input arr2ing on FIFO
        // and close it
        write(fd, resp, strlen(resp)+1);
        close(fd);
        // Open FIFO for Read only
        fd = open(myfifo, O_RDONLY);

        // Read from FIFO
        read(fd, arr1, sizeof(arr1));

        // Print the read message
        printf("Server: %s\n", arr1);
        close(fd);

    }
    return 0;
}
