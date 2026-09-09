#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        // CHILD
        printf("Child: PID = %d\n", getpid());
        printf("Child: I will execute ls -l\n");
        fflush(stdout);

        char *args[] = {"/bin/ls", "-l", NULL};
        char *env[] = {NULL};

        execve("/bin/ls", args, env);

        // Only executed if execve() fails
        perror("execve");
        return 1;
    }

    // PARENT
    printf("Parent: PID = %d\n", getpid());
    printf("Parent: waiting for child %d...\n", pid);

    waitpid(pid, NULL, 0);

    printf("Parent: child finished.\n");

    return 0;
}