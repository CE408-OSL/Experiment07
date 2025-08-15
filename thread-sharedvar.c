#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <pthread.h>

int oslab;

void* func(void* args){
    printf("before in kid:%d\n", oslab);
    oslab=2;
    printf("after in kid:%d\n", oslab);
    return NULL;
}   

int main() {
    oslab=1;
    pthread_t thread;
    printf("before in main:%d\n", oslab);
    pthread_create(&thread, NULL, func, NULL);
    pthread_join(thread, NULL);
    printf("after in main:%d\n", oslab);
    return 0;
}
