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
    printf("hello world (pid:%d)\n", (int) getpid());
    int fd = open("out.txt", O_CREAT | O_TRUNC | O_WRONLY /* | O_APPEND */, 0644);
    if (fd < 0) { perror("open"); exit(1); }

    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
	close(fd);
        exit(1);
    } else if (rc == 0) {
        // child (new process)
        printf("hello, I am child (pid:%d)\n", (int) getpid());
	const char *msg = "child\n";
	for (int i = 0; i < 10; i++) {
	    if (write(fd, msg, (int)strlen(msg)) < 0) { perror("write(child)"); _exit(1); }
	    sleep(1);
	}
	close(fd);
	_exit(0);
    } else {
        // parent goes down this path (original process)
      	const char *msg = "parent\n";
        for (int i = 0; i < 10; i++) {
            if (write(fd, msg, (int)strlen(msg)) < 0) { perror("write(parent)"); exit(1); }
            sleep(1);
        }
        close(fd);
        int wc = wait(NULL);
        assert(wc >= 0);
    }
    return 0;
}
