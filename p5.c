#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>
#include <errno.h>

int
main(int argc, char *argv[])
{
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child has no children; wait() should fail with ECHILD
        int st;
        pid_t r = wait(&st);
        if (r == -1) perror("child wait (expected ECHILD)");
        _exit(42); // exit code 42
    } else {
        int st;
        pid_t done = wait(&st);
        assert(done >= 0);
        if (WIFEXITED(st)) {
            printf("parent: child %d exited with code %d\n",
                   (int)done, WEXITSTATUS(st));
        } else if (WIFSIGNALED(st)) {
            printf("parent: child %d killed by signal %d\n",
                   (int)done, WTERMSIG(st));
        }
    }
    return 0;
}
