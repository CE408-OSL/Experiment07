#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <pthread.h>

struct thdata {
    int thread_no;
    char message[100];
} stdata;

void* func(void* params) {
    struct thdata data = *((struct thdata*) params);
    printf("id:%d msg:%s\n", data.thread_no, data.message);
}

int main() {
    pthread_t threads[2];

    struct thdata data[2] = {
        {1, "thread 1"},
        {2, "thread 2"},
    };
    
    for (size_t i = 0; i < 2; i++) {
        pthread_create(&threads[i], NULL, func, data+i);
    }
    
    for (size_t i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
