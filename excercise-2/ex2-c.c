#include <stdio.h>
#include <pthread.h>

const int iterations = 1000000;

int accumulator = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *increase(){
    for(int i = 0; i < iterations; i++)
    {
        pthread_mutex_lock(&lock);
        accumulator++;
        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

void *decrease(){
    for(int i = 0; i < iterations; i++)
    {
        pthread_mutex_lock(&lock);
        accumulator--;
        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

int main()
{
    pthread_t thread_inc;
    pthread_t thread_dec;

    pthread_create(&thread_inc, NULL, increase, NULL);
    pthread_create(&thread_dec, NULL, decrease, NULL);

    pthread_join(thread_inc, NULL);
    pthread_join(thread_dec, NULL);

    printf("%d\n", accumulator);
    return 0;
}
