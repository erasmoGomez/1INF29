#include <stdio.h>
#include <unistd.h>

int main(void) {

    char *args[] = {"/bin/ls", "-l", NULL};
    char *env[] = {NULL};

    printf("Before execve()\n");

    execve("/bin/ls", args, env);

    // This line only runs if execve() fails
    perror("execve failed");

    return 1;
}