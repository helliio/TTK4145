#include <stdio.h>
#include <pthread.h>

const int iterations = 1000000;

int accumulator = 0;

void *increase(){
    for(int i = 0; i < iterations; i++)
    {
        accumulator++;
    }

    return NULL;
}

void *decrease(){
    for(int i = 0; i < iterations; i++)
    {
        accumulator--;
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
