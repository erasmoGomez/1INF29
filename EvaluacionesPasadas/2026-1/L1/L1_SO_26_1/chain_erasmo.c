#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    const int chain_length = 3;
    int level = 0;

    printf("Root process: %d\n", getpid());

    for (int i = 0; i < chain_length; ++i) {
        sleep(5);

        const pid_t child_pid = fork();

        if (child_pid < 0) {
            perror("fork");
            return EXIT_FAILURE;
        }

        if (child_pid > 0) {
            printf("%*sProcess %d created child %d\n",
                   level * 4, "", getpid(), child_pid);
            break;
        }

        ++level;

        printf("%*s└── Child %d started (parent: %d)\n",
               level * 4, "", getpid(), getppid());
    }

    wait(NULL);

    printf("%*sProcess %d finished\n",
           level * 4, "", getpid());

    return EXIT_SUCCESS;
}