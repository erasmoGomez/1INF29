#include <stdio.h>
#include <unistd.h>

int main(void) {

    char *args[] = {"/bin/does_not_exist", NULL};
    char *env[] = {NULL};

    printf("Before execve()\n");

    execve("/bin/does_not_exist", args, env);

    // This runs because execve() failed
    perror("execve");

    printf("The original program continues running.\n");

    return 1;
}