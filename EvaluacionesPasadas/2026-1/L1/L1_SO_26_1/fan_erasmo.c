#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    const int number_of_children = 4;
    const pid_t root_pid = getpid();
    printf("Parent %d\n", root_pid);

    // Build the fan.
    for (int i = 0; i < number_of_children; ++i) {
        sleep(2);

        const pid_t child_pid = fork();

        if (child_pid < 0) {
            perror("fork");
            return EXIT_FAILURE;
        }

        if (child_pid == 0)
            break;

        printf("Created child %d\n", child_pid);
    }

    // Keep every process alive for a few seconds.
    sleep(4);

    // The root process waits for all its children.
    if (getpid() == root_pid) {
        for (int i = 0; i < number_of_children; ++i)
            wait(NULL);
    }

    printf("Process %d finished (parent: %d)\n",
           getpid(), getppid());

    return EXIT_SUCCESS;
}