#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>

extern char **environ;

int
main(int argc, char *argv[])
{
    const char *which = (argc > 1) ? argv[1] : "execl";

    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        if (strcmp(which, "execl") == 0) {
            execl("/bin/ls", "ls", "-l", "/", (char*)NULL);
        } else if (strcmp(which, "execle") == 0) {
            char *envp[] = { "LANG=C", "FOO=BAR", NULL };
            execle("/bin/ls", "ls", "-l", "/", (char*)NULL, envp);
        } else if (strcmp(which, "execlp") == 0) {
            execlp("ls", "ls", "-l", "/", (char*)NULL);
        } else if (strcmp(which, "execv") == 0) {
            char *args[] = { "ls", "-l", "/", NULL };
            execv("/bin/ls", args);
        } else if (strcmp(which, "execvp") == 0) {
            char *args[] = { "ls", "-l", "/", NULL };
            execvp("ls", args);
        } else if (strcmp(which, "execvpe") == 0) { // GNU extension
            char *args[] = { "ls", "-l", "/", NULL };
            char *envp[] = { "LANG=C", "FOO=BAZ", NULL };
            execvpe("ls", args, envp);
        } else {
            fprintf(stderr, "usage: %s [execl|execle|execlp|execv|execvp|execvpe]\n", argv[0]);
            _exit(2);
        }
        perror("exec"); // reach here only if exec failed
        _exit(1);
    } else {
        int wc = wait(NULL);
        assert(wc >= 0);
    }
    return 0;
}
