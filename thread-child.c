#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int global_var = 0;

void *child(void *arg) {
    int local_var;

    printf("Thread %ld, pid %d, addresses: &global: %p, &local: %p \n",
        pthread_self(), getpid(), &global_var, &local_var);

    global_var++;

    printf("Thread %ld, pid %d, incremented global var=%d\n",
        pthread_self(), getpid(), global_var);

    pthread_exit(0);
}

int main(void) {
    pthread_t t1, t2;

    global_var = 0;

    if (pthread_create(&t1, NULL, child, NULL) != 0) {
        perror("pthread_create t1");
        return 1;
    }

    if (pthread_create(&t2, NULL, child, NULL) != 0) {
        perror("pthread_create t2");
        return 1;
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Main thread: final global_var=%d\n", global_var);
    return 0;
}