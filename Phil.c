#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

#define NUM_PHILOSOPHERS 5
#define LEFT (id + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS
#define RIGHT (id + 1) % NUM_PHILOSOPHERS

enum {THINKING, HUNGRY, STARVING, EATING} status[NUM_PHILOSOPHERS];
sem_t phil[NUM_PHILOSOPHERS];
sem_t mutex;
time_t timeWaited[NUM_PHILOSOPHERS];

void test_forks_available(int id) {
    if ((status[id] == HUNGRY && status[LEFT] < STARVING && status[RIGHT] < STARVING) || status[id] == STARVING) {
        status[id] = EATING;
        sem_post(&phil[id]);
    }
    else if (status[LEFT] == STARVING || status[RIGHT] == STARVING) {
            printf("Philosopher %d fasted a little longer in favor of a fellow student.\n", id);
    }
}

void prevent_starvation(int id) {
    if (status[id] == HUNGRY && (status[LEFT] == EATING || status[RIGHT] == EATING)) {
        status[id] = STARVING;
    }
}

void grab(int id) {
    sem_wait(&mutex);
    status[id] = HUNGRY;
    test_forks_available(id);
    sem_post(&mutex);
    sem_wait(&phil[id]);
}

void drop(int id) {
    sem_wait(&mutex);
    status[id] = THINKING;
    printf("Philosopher %d is done eating. And waited %ld seconds to do so.\n", id, time(NULL) - timeWaited[id]);
    timeWaited[id] = time(NULL);
    test_forks_available(LEFT);
    test_forks_available(RIGHT);
    prevent_starvation(LEFT);
    prevent_starvation(RIGHT);
    sem_post(&mutex);
}

void *philosopher(void *arg) {
    int id = *((int *)arg);

    timeWaited[id] = time(NULL);

    while (1) {
        printf("Philosopher %d is thinking.\n", id);
        sleep(rand() % 4);
        grab(id);
        printf("Philosopher %d is eating.\n", id);
        sleep(rand() % 4);
        drop(id);
    }
}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int philosopher_ids[NUM_PHILOSOPHERS];
    int i;

    sem_init(&mutex, 0, 1);

    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_init(&phil[i], 0, 0);
        philosopher_ids[i] = i;
    }

    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_ids[i]);
    }

    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    sem_destroy(&mutex);
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_destroy(&phil[i]);
    }

    return 0;
}
