#define _DEFAULT_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define N_READERS 4
#define N_WRITERS 2

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t resource = PTHREAD_MUTEX_INITIALIZER;
int readers = 0;
int shared_value = 0;

void *reader(void *arg) {
    int id = *(int *)arg;

    pthread_mutex_lock(&mutex);
    readers++;
    if (readers == 1) pthread_mutex_lock(&resource); // First reader blocks writers.
    pthread_mutex_unlock(&mutex);

    printf("Reader %d reads shared_value = %d (%d readers active).\n", id, shared_value, readers);
    usleep(200000);

    pthread_mutex_lock(&mutex);
    readers--;
    if (readers == 0) pthread_mutex_unlock(&resource); // Last reader releases resource.
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *writer(void *arg) {
    int id = *(int *)arg;
    pthread_mutex_lock(&resource);
    shared_value += 10;
    printf("Writer %d writes shared_value = %d.\n", id, shared_value);
    usleep(250000);
    pthread_mutex_unlock(&resource);
    return NULL;
}

int main(void) {
    pthread_t r[N_READERS], w[N_WRITERS];
    int rid[N_READERS], wid[N_WRITERS];

    for (int i = 0; i < N_READERS; i++) {
        rid[i] = i + 1;
        pthread_create(&r[i], NULL, reader, &rid[i]);
    }
    usleep(50000);
    for (int i = 0; i < N_WRITERS; i++) {
        wid[i] = i + 1;
        pthread_create(&w[i], NULL, writer, &wid[i]);
    }

    for (int i = 0; i < N_READERS; i++) pthread_join(r[i], NULL);
    for (int i = 0; i < N_WRITERS; i++) pthread_join(w[i], NULL);

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&resource);
    return 0;
}
