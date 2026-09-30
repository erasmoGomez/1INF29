#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();

    if (pid == 0) {
        pid_t pid2 = fork();

        if (pid2 == 0) {
            printf("Grandchild\n");
        } else {
            waitpid(pid2, NULL, 0);
            printf("Child\n");
        }

    } else {
        waitpid(pid, NULL, 0);
        printf("Parent\n");
    }

    return 0;
}