#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

void *myFunc(void *vargp) {
    printf("Printing from thread\n");
    return NULL;
}

int main() {
    pid_t pid;
    pthread_t tid;

    pid = fork();
    if (pid == 0) { /* CHILD */
       // fork();
        pthread_create(&tid, NULL, myFunc, NULL);
        pthread_join(tid,NULL);
    }
    fork();
}
