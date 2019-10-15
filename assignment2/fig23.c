#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int value = 0;
void *runner(void *param); /* the therad */

int main(int argc, char *argv[]) {
    pid_t pid;
    pthread_t tid;
    pthread_attr_t attr;

        pid = fork();

        if (pid == 0) { /* CHILD PROCESS */
            pthread_attr_init(&attr);
            pthread_create(&tid, &attr, runner, NULL);
            pthread_join(tid, NULL);
            printf("CHILD: value = %d", value); /* LINE C */
        } else if (pid > 0) { /* PARENT PROCESS */
            wait(NULL);
            printf("PARENT: value = %d", value); /* LINE P*/
        }
    return 0;
}

void *runner(void *param) {
    value = 5;
    pthread_exit(0);
}
