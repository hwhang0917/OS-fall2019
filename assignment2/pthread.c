#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for sleep()
#include <pthread.h>

void *myFunc(void *vargp) {
    sleep(1);
    printf("Printing GeeksQuiz from Thread \n");
    return NULL;
}

