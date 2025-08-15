#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <pthread.h>

void* func(void* args) {
    printf("Hello World!\n");
    pthread_exit(0);
}

int main() {
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int n;
    scanf("%d", &n);
    pthread_t threads[n];
    
    for (size_t i = 0; i < n; i++) {
        pthread_create(&threads[i], &attr, func, NULL);
    }

    for (size_t i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
