#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void usage() {
    printf ("sendtty: tty sendstring\n");
    exit(0);
}

int main(int argc, char** argv) {
    if (argc != 3)
        usage();

    char * ttypath = argv[1];
    char * sendstring = argv[2];

    int ttyfd = open (ttypath, O_WRONLY | O_NONBLOCK);
    if (ttyfd < 0) {
        perror("ERROR");
        return 1;
    }
    if (!isatty(ttyfd)) {
        fprintf (stderr,"ERROR: %s is not a tty\n",ttypath);
        return 1;
    }
    // Interpret format string

    while (*sendstring != '\0') {
        char putchar;

        putchar = *sendstring;
        // Detect escape sequence
        if (putchar == '\\') {
            sendstring++;
            putchar = *sendstring;
            if (putchar == 'n')
                putchar = '\n';
        }
        if (ioctl (ttyfd, TIOCSTI, &putchar)) {
            perror("ERROR");
            return 1;
        }
        sendstring++;
    }

    close (ttyfd);
    return 0;
}
