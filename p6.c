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
    int rcA = fork();
    if (rcA < 0) { fprintf(stderr, "fork A failed\n"); exit(1); }
    if (rcA == 0) { usleep(200000); _exit(10); } // child A exits later

    int rcB = fork();
    if (rcB < 0) { fprintf(stderr, "fork B failed\n"); exit(1); }
    if (rcB == 0) { usleep(100000); _exit(20); } // child B exits earlier

    int st;
    pid_t r = waitpid(rcA, &st, 0);         // specifically wait for A first
    assert(r == rcA);
    printf("waited for A (%d), exit=%d\n", rcA,
           WIFEXITED(st) ? WEXITSTATUS(st) : -1);

    r = waitpid(-1, &st, 0);                // reap the remaining child
    assert(r == rcB);
    printf("waited for B (%d), exit=%d\n", rcB,
           WIFEXITED(st) ? WEXITSTATUS(st) : -1);

    return 0;
}

