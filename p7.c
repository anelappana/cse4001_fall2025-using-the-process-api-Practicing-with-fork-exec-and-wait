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
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        close(STDOUT_FILENO);
        printf("you should NOT see this line (child pid:%d)\n", (int)getpid());
        if (fflush(stdout) != 0) perror("child fflush"); // expect EBADF
        _exit(0);
    } else {
        printf("parent still has stdout (pid:%d, child:%d)\n", (int)getpid(), rc);
        int wc = wait(NULL);
        assert(wc >= 0);
    }
    return 0;
}
