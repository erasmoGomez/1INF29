#include <stdio.h>

#define TASKS 3

typedef struct {
    char name[4];
    int execution;
    int period;
    int remaining;
    int deadline;
    int next_release;
} Task;

int main(void) {
    Task t[TASKS] = {
        {"T1", 1, 4, 0, 0, 0},
        {"T2", 1, 5, 0, 0, 0},
        {"T3", 2,10, 0, 0, 0}
    };

    printf("Simple EDF simulation for t=0..19\n");
    for (int time = 0; time < 20; time++) {
        for (int i = 0; i < TASKS; i++) {
            if (time == t[i].next_release) {
                if (t[i].remaining > 0)
                    printf("t=%d DEADLINE MISS by %s\n", time, t[i].name);
                t[i].remaining = t[i].execution;
                t[i].deadline = time + t[i].period;
                t[i].next_release += t[i].period;
            }
        }

        int best = -1;
        for (int i = 0; i < TASKS; i++)
            if (t[i].remaining > 0 && (best == -1 || t[i].deadline < t[best].deadline)) best = i;

        if (best == -1) printf("t=%2d: IDLE\n", time);
        else {
            printf("t=%2d: run %s (deadline=%d)\n", time, t[best].name, t[best].deadline);
            t[best].remaining--;
        }
    }
    return 0;
}
