#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const int n = atoi(argv[1]);

    if (n <= 0) {
        fprintf(stderr, "Error: n must be a positive integer.\n");
        return EXIT_FAILURE;
    }

    const pid_t root_pid = getpid();
    int fan_id = 0;
    int chain_level = 0;

    printf("ROOT: PID %d\n", root_pid);
    printf("\n=== BUILDING FAN ===\n");
    fflush(stdout);

    // Build the fan.
    for (int i = 0; i < n; ++i) {
        sleep(1);

        const pid_t child_pid = fork();

        if (child_pid < 0) {
            perror("fork");
            return EXIT_FAILURE;
        }

        if (child_pid == 0) {
            fan_id = i + 1;

            printf("  └── Fan %d: PID %d, PPID %d\n",
                   fan_id, getpid(), getppid());
            fflush(stdout);

            break;
        }

        printf("ROOT %d created Fan %d: PID %d\n",
               root_pid, i + 1, child_pid);
        fflush(stdout);
    }

    // Each fan child builds its own chain.
    if (getpid() != root_pid) {
        // Wait until the root finishes building the complete fan.
        sleep(n + 1);

        printf("\n[Fan %d] Building chain from PID %d\n",
               fan_id, getpid());
        fflush(stdout);

        for (int i = 0; i < n; ++i) {
            sleep(1);

            const pid_t child_pid = fork();

            if (child_pid < 0) {
                perror("fork");
                return EXIT_FAILURE;
            }

            if (child_pid > 0) {
                printf("[Fan %d] %*sPID %d created child %d\n",
                       fan_id,
                       chain_level * 4, "",
                       getpid(), child_pid);
                fflush(stdout);

                break;
            }

            ++chain_level;

            printf("[Fan %d] %*s└── PID %d, PPID %d\n",
                   fan_id,
                   chain_level * 4, "",
                   getpid(), getppid());
            fflush(stdout);
        }
    }

    // Keep all processes alive for observation.
    pause();

    return EXIT_SUCCESS;
}