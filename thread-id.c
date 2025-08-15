#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <pthread.h>

void* func(void* args){
    pid_t id = pthread_self();
    printf("child id:%d\n", id);
    return NULL;
}

int main() {
    pthread_t thread;
    pid_t id = pthread_self();
    printf("main id: %d\n", id);
    pthread_create(&thread, NULL, func, NULL);
    pthread_join(thread, NULL);
    return 0;
}
