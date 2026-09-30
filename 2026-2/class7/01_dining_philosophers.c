#define _DEFAULT_SOURCE
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define N 5

sem_t forks[N];
sem_t room;

void *philosopher(void *arg) {
    int id = *(int *)arg;
    int left = id;
    int right = (id + 1) % N;

    printf("Philosopher %d is thinking.\n", id);
    usleep(150000);

    sem_wait(&room);              // At most N-1 philosophers try to take forks.
    sem_wait(&forks[left]);
    printf("Philosopher %d took fork %d.\n", id, left);
    sem_wait(&forks[right]);

    printf("Philosopher %d is EATING with forks %d and %d.\n", id, left, right);
    usleep(250000);

    sem_post(&forks[right]);
    sem_post(&forks[left]);
    sem_post(&room);

    printf("Philosopher %d finished eating.\n", id);
    return NULL;
}

int main(void) {
    pthread_t threads[N];
    int ids[N];

    sem_init(&room, 0, N - 1);
    for (int i = 0; i < N; i++) sem_init(&forks[i], 0, 1);

    for (int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, philosopher, &ids[i]);
    }
    for (int i = 0; i < N; i++) pthread_join(threads[i], NULL);

    for (int i = 0; i < N; i++) sem_destroy(&forks[i]);
    sem_destroy(&room);
    return 0;
}
