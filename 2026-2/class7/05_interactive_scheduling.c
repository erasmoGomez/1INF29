#include <stdio.h>

#define N 4

typedef struct {
    char name[4];
    int burst;
    int priority; // Smaller number = higher priority.
} Process;

void round_robin(Process p[], int quantum) {
    int remaining[N], time = 0, unfinished = N;
    for (int i = 0; i < N; i++) remaining[i] = p[i].burst;

    printf("Round Robin, quantum=%d\n", quantum);
    while (unfinished) {
        for (int i = 0; i < N; i++) {
            if (remaining[i] == 0) continue;
            int slice = remaining[i] < quantum ? remaining[i] : quantum;
            printf("| %s %d-%d ", p[i].name, time, time + slice);
            time += slice;
            remaining[i] -= slice;
            if (remaining[i] == 0) unfinished--;
        }
    }
    printf("|\n");
}

void priority_scheduling(Process p[]) {
    int done[N] = {0}, time = 0;
    printf("Priority scheduling (non-preemptive)\n");
    for (int count = 0; count < N; count++) {
        int best = -1;
        for (int i = 0; i < N; i++)
            if (!done[i] && (best == -1 || p[i].priority < p[best].priority)) best = i;
        printf("| %s[p=%d] %d-%d ", p[best].name, p[best].priority, time, time + p[best].burst);
        time += p[best].burst;
        done[best] = 1;
    }
    printf("|\n");
}

int main(void) {
    Process p[N] = {{"P1",7,3},{"P2",4,1},{"P3",5,4},{"P4",2,2}};
    round_robin(p, 2);
    round_robin(p, 4);
    priority_scheduling(p);
    printf("\nDiscussion: smaller quantum improves response time but increases context switches.\n");
    printf("Priority scheduling may starve low-priority processes; aging is a common remedy.\n");
    return 0;
}
