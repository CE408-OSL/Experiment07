#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int global_param;

void *child(void *param) {
    int local_var;

    printf("Thread %ld, pid %d, addresses: &global: %p, &local: %p \n",
        pthread_self(), getpid(), &global_param, &local_var);

    global_param++;

    printf("Thread %ld, pid %d, incremented global param=%d\n",
        pthread_self(), getpid(), global_param);

    pthread_exit(0);
}

int main(){    
    global_param = 100;
    pthread_t threads[2];
    
    pthread_create(&threads[0], NULL, child, NULL);
    pthread_create(&threads[1], NULL, child, NULL);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    printf("the final global param : %d\n", global_param);
    
    global_param = 200;
    int local_param = 999;

    int k = fork();
    if (k == 0){
        printf("Child: local and global param before change: %d, %d\n", local_param, global_param);
        local_param = 111;
        global_param = 4321;
        printf("Child: local and global param after change: %d, %d\n", local_param, global_param);
    }
    else{
        printf("Parent: local and global param before change: %d, %d\n", local_param, global_param);
        wait(NULL);
        printf("Parent: local and global param after change: %d, %d\n", local_param, global_param);
    }
    return 0;
}
