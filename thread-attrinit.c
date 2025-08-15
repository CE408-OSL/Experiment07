#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <pthread.h>

void* func(void* args) {
    int num = (int)(size_t) args;
    int javab = 0;
    for (int i = 2; i < num + 1; i++) {
        javab = javab + i;
    }
    printf("sum: %d\n", javab);
    return NULL;
}

int main() {
    int n;
    scanf("%d", &n);
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_t thread;
    pthread_create(&thread, &attr, func, (void *)(size_t) n);
    pthread_join(thread, NULL);
    return 0;
}
