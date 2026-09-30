#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <string.h>

void *work(void *arg) {
    int id = *(int *)arg;
    int policy;
    struct sched_param param;
    pthread_getschedparam(pthread_self(), &policy, &param);
    printf("Thread %d -> policy=%s, priority=%d\n", id,
           policy == SCHED_FIFO ? "SCHED_FIFO" : policy == SCHED_RR ? "SCHED_RR" : "SCHED_OTHER",
           param.sched_priority);
    for (volatile long i = 0; i < 80000000L; i++);
    printf("Thread %d finished.\n", id);
    return NULL;
}

int main(void) {
    pthread_t th[3];
    int ids[3] = {1,2,3};
    printf("The OS normally decides execution order under SCHED_OTHER.\n");
    for (int i = 0; i < 3; i++) pthread_create(&th[i], NULL, work, &ids[i]);
    for (int i = 0; i < 3; i++) pthread_join(th[i], NULL);
    printf("Run several times: output order may vary. Real-time policies often require privileges.\n");
    return 0;
}
