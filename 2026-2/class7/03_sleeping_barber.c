#define _DEFAULT_SOURCE
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define CHAIRS 3
#define CUSTOMERS 8

sem_t customers;
sem_t barber_ready;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int waiting = 0;
int shop_open = 1;

void *barber(void *arg) {
    (void)arg;
    while (1) {
        sem_wait(&customers); // Barber sleeps here when nobody is waiting.

        pthread_mutex_lock(&mutex);
        if (!shop_open && waiting == 0) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        waiting--;
        printf("Barber calls a customer. Waiting now: %d\n", waiting);
        sem_post(&barber_ready);
        pthread_mutex_unlock(&mutex);

        printf("Barber is cutting hair...\n");
        usleep(300000);
    }
    printf("Barber closes the shop.\n");
    return NULL;
}

void *customer(void *arg) {
    int id = *(int *)arg;
    pthread_mutex_lock(&mutex);
    if (waiting < CHAIRS) {
        waiting++;
        printf("Customer %d sits down. Waiting: %d\n", id, waiting);
        sem_post(&customers);
        pthread_mutex_unlock(&mutex);

        sem_wait(&barber_ready);
        printf("Customer %d is being served.\n", id);
    } else {
        printf("Customer %d leaves: no free chair.\n", id);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(void) {
    pthread_t barber_thread, c[CUSTOMERS];
    int ids[CUSTOMERS];

    sem_init(&customers, 0, 0);
    sem_init(&barber_ready, 0, 0);
    pthread_create(&barber_thread, NULL, barber, NULL);

    for (int i = 0; i < CUSTOMERS; i++) {
        ids[i] = i + 1;
        pthread_create(&c[i], NULL, customer, &ids[i]);
        usleep(70000);
    }
    for (int i = 0; i < CUSTOMERS; i++) pthread_join(c[i], NULL);

    pthread_mutex_lock(&mutex);
    shop_open = 0;
    pthread_mutex_unlock(&mutex);
    sem_post(&customers); // Wake barber so it can terminate.
    pthread_join(barber_thread, NULL);

    sem_destroy(&customers);
    sem_destroy(&barber_ready);
    pthread_mutex_destroy(&mutex);
    return 0;
}
