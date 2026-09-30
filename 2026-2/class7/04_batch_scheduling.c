#include <stdio.h>
#include <limits.h>

#define N 4

typedef struct {
    char name[4];
    int arrival;
    int burst;
} Process;

void print_metrics(const char *name, Process p[], int completion[]) {
    double total_wait = 0, total_turnaround = 0;
    printf("\n%s\n", name);
    printf("Process  Arrival  Burst  Completion  Turnaround  Waiting\n");
    for (int i = 0; i < N; i++) {
        int turnaround = completion[i] - p[i].arrival;
        int waiting = turnaround - p[i].burst;
        total_wait += waiting;
        total_turnaround += turnaround;
        printf("%-7s  %7d  %5d  %10d  %10d  %7d\n",
               p[i].name, p[i].arrival, p[i].burst, completion[i], turnaround, waiting);
    }
    printf("Average waiting: %.2f | Average turnaround: %.2f\n",
           total_wait / N, total_turnaround / N);
}

void fcfs(Process p[]) {
    int completion[N], time = 0;
    printf("\nGantt FCFS: ");
    for (int i = 0; i < N; i++) {
        if (time < p[i].arrival) time = p[i].arrival;
        printf("| %s %d-%d ", p[i].name, time, time + p[i].burst);
        time += p[i].burst;
        completion[i] = time;
    }
    printf("|\n");
    print_metrics("FCFS", p, completion);
}

void sjf(Process p[]) {
    int done[N] = {0}, completion[N] = {0}, time = 0, finished = 0;
    printf("\nGantt SJF: ");
    while (finished < N) {
        int best = -1;
        for (int i = 0; i < N; i++)
            if (!done[i] && p[i].arrival <= time && (best == -1 || p[i].burst < p[best].burst)) best = i;
        if (best == -1) { time++; continue; }
        printf("| %s %d-%d ", p[best].name, time, time + p[best].burst);
        time += p[best].burst;
        completion[best] = time;
        done[best] = 1;
        finished++;
    }
    printf("|\n");
    print_metrics("SJF (non-preemptive)", p, completion);
}

void srtn(Process p[]) {
    int remaining[N], completion[N] = {0}, finished = 0, time = 0, last = -1;
    for (int i = 0; i < N; i++) remaining[i] = p[i].burst;
    printf("\nTimeline SRTN: ");
    while (finished < N) {
        int best = -1;
        for (int i = 0; i < N; i++)
            if (p[i].arrival <= time && remaining[i] > 0 && (best == -1 || remaining[i] < remaining[best])) best = i;
        if (best == -1) { time++; continue; }
        if (best != last) printf("| t=%d -> %s ", time, p[best].name);
        last = best;
        remaining[best]--;
        time++;
        if (remaining[best] == 0) {
            completion[best] = time;
            finished++;
        }
    }
    printf("| end=%d |\n", time);
    print_metrics("SRTN / SRTF", p, completion);
}

int main(void) {
    Process p[N] = {{"P1",0,8},{"P2",1,4},{"P3",2,2},{"P4",3,1}};
    printf("Same workload, three batch schedulers. Change arrival/burst values and compare.\n");
    fcfs(p);
    sjf(p);
    srtn(p);
    return 0;
}
