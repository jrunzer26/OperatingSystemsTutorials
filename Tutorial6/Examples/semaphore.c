
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <pthread.h>
#include <semaphore.h>

sem_t sem;

int count = 0;

void *runner(void *arg)
{
    char *str;
    int i = 0;

    str=(char*)arg;

    while(i < 10)
    {
        // Create a critical section
        sem_wait(&sem);

        // Increase the count
        count++;
        printf("%s says %i\n",str, count);

        // End of the critical section
        sem_post(&sem);

        usleep(1);
        ++i;
    }

    return NULL;
}

/*
 * Good info on pthreads semaphores from: http://www.csc.villanova.edu/~mdamian/threads/posixsem.html
 * Use semaphores to simulate the mutex example of
 * incrementing a shared variable.
 */
int main(int argc, char *argv[]) {

    pthread_t pth;
    pthread_t pth2;

    // Initialize the semaphore.
    sem_init(&sem, 0, 1);

    // Create the threads
    pthread_create(&pth, 0, runner, (void *) "Thread 1");
    pthread_create(&pth2, 0, runner, (void *) "Thread 2");

    // wait for our thread to finish before continuing
    pthread_join(pth, 0);
    pthread_join(pth2, 0);

    // Clean up
    sem_destroy(&sem);

    return 0;
}