// p3_child_first_no_wait.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
    int p[2];
    if (pipe(p) < 0) { perror("pipe"); exit(1); }

    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        close(p[0]); // child writes signal after printing
        printf("hello (child pid:%d)\n", (int)getpid());
        fflush(stdout);
        if (write(p[1], "x", 1) != 1) perror("write");
        close(p[1]);
        _exit(0);
    } else {
        close(p[1]); // parent waits by reading one byte
        char b;
        if (read(p[0], &b, 1) != 1) perror("read");
        close(p[0]);
        printf("goodbye (parent pid:%d, child:%d)\n", (int)getpid(), rc);
        // no wait(); child already exited or will soon; pipe enforced print order
    }
    return 0;
}
