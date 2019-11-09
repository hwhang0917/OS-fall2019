#include <iostream>
#include <stdlib.h> // required for rand()
#include <unistd.h> // sleep()
#include <pthread.h>
#include <semaphore.h>

#define MAX_BUFFER_SIZE 10
#define MAX_THREAD_NUMBER 20

void *consumer(void *param);
void *producer(void *param);
void insert_item(int in_item);
void remove_item(int *out_item);

typedef struct {
    sem_t sem;
    int count;
} semaphore;

int buffer_pool[MAX_BUFFER_SIZE]; // shared buffer pool
int in = 0, out = 0;

semaphore empty, full; // two general semaphores
pthread_mutex_t prod_mutex, cons_mutex; // two mutexes

int main() {   
    // Initialize necessary vars
    int sleep_sec, prod_num, cons_num;

    int in_item, out_item;
    empty.count = 10; full.count = 0;

    // Initialize Semaphores
    sem_init(&empty.sem, MAX_BUFFER_SIZE, MAX_BUFFER_SIZE); // empty count is initially 10
    sem_init(&full.sem, 0, MAX_BUFFER_SIZE); // full count is initially 0
    pthread_mutex_init(&prod_mutex, NULL);
    pthread_mutex_init(&cons_mutex, NULL);
    pthread_t tid_producer[MAX_THREAD_NUMBER], tid_consumer[MAX_THREAD_NUMBER];

    // Take user input
    std::cout << "Enter the number of seconds for the main() function to sleep: "; std::cin >> sleep_sec;
    do {
        std::cout << "Enter the number of producer threads: "; std::cin >> prod_num;
        std::cout << "Enter the number of consumer threads: "; std::cin >> cons_num;

        // Maximum of 20 threads can be created
        if (prod_num > MAX_THREAD_NUMBER || cons_num > MAX_THREAD_NUMBER)
            std::cout << "ERROR: Try entering limited thread number (<20)" << std::endl;

    } while (prod_num > MAX_THREAD_NUMBER || cons_num > MAX_THREAD_NUMBER);

    // Creating Threads
    for (int i = 0; i < prod_num; i++) { // Create number of producer threads
        pthread_create(&tid_producer[i], NULL, producer, NULL);
    }
    for (int i = 0; i < cons_num; i++) { // Create number of consumer threads
        pthread_create(&tid_consumer[i], NULL, consumer, NULL);
    }

    sleep(sleep_sec); // Sleep main thread for input seconds

    // // Joining Threads
    // for (int i = 0; i < prod_num; i++) { // Wait number of producer threads
    //     pthread_join(tid_producer[i], NULL);
    // }
    // for (int i = 0; i < cons_num; i++) { // Wait number of consumer threads
    //     pthread_join(tid_consumer[i], NULL);
    // }

    // // Destroy semaphores and mutexes
    // pthread_mutex_destroy(&prod_mutex);
    // pthread_mutex_destroy(&cons_mutex);
    // sem_destroy(&empty.sem);
    // sem_destroy(&full.sem);

    std::cout << "Main thread finish" << std::endl;

    return 0;
}

void insert_item(int in_item) {
    sem_wait(&empty.sem); // make sure buffer pool is not full
    pthread_mutex_lock(&prod_mutex); // synchronize the multiple producer threads

    //~~~~~~~~ CRITICAL SECTION
    buffer_pool[in] = in_item;
    in = (in + 1) % MAX_BUFFER_SIZE; // update the in pointer
    //~~~~~~~~~~~~~~~~~~~~~~~~~ END

    pthread_mutex_unlock(&prod_mutex); // synchronize the multiple producer threads
    sem_post(&full.sem); // notify the consumer threads
}

void remove_item(int *out_item) {
    sem_wait(&full.sem); // make sure buffer pool is not empty
    pthread_mutex_lock(&cons_mutex); // synchronize the multiple consumer threads

    //~~~~~~~~ CRITICAL SECTION
    *out_item = buffer_pool[out];
    out = (out + 1) % MAX_BUFFER_SIZE; // updat ethe out pointer
    //~~~~~~~~~~~~~~~~~~~~~~~~~ END

    pthread_mutex_unlock(&cons_mutex); // synchronize the multiple producer threads
    sem_post(&empty.sem); // notify the producer threads
}

void *producer (void *param) {
    int item;
    pthread_t tid = pthread_self();

    while (true) {
        sleep(rand() % 3); // generate a random number for the producer thread to sleep
        item = rand() % 20; // generate a random number representing an item to be deposited
        insert_item(item); // semaphores and mutex are implemented in the function
        std::cout << "producer " << tid << " produced " << item << " in buffer " << in << std::endl;
    }
}

void *consumer(void *param) {
    int item;
    pthread_t tid = pthread_self();

    while(true) {
        sleep(rand() % 3); // generate a random number for the producer thread to sleep
        remove_item(&item); // semaphores and mutex are implemented in the function
        std::cout << "consumer " << tid << " consumed " << item << " from buffer " << out << std::endl;
    }
}