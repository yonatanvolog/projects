/*
Best time: 125 threads

compile with: -pthread
*/

#include <stdio.h>
#include <pthread.h>

#define NUM_OF_THREADS (125)

typedef struct wrapper_t
{
    size_t num;
    size_t start;
    size_t end;
    size_t sum;
}wrapper_t;

void *SumOfDividorsInRange(void *wrapper)
{
    size_t i = 0;
    wrapper_t *unwrapper = (wrapper_t *)wrapper;
  
    for (i = unwrapper->start; i <= unwrapper->end; ++i)
    {
        if (0 == unwrapper->num % i)
        {
            unwrapper->sum += i;
        }
    }
}

int main()
{
    size_t total_sum = 0;
    size_t i = 0;
    size_t num = 1000000000;
    int status = 1;
    
    wrapper_t wrapper[NUM_OF_THREADS] = {0};
    pthread_t thread[NUM_OF_THREADS] = {0};
    
    for (i = 0; i < NUM_OF_THREADS; ++i)
    {
        status = 1;
        wrapper[i].num = num;
        wrapper[i].start = (num / NUM_OF_THREADS) * i + 1;
        wrapper[i].end = (num / NUM_OF_THREADS) * (i + 1);
        wrapper[i].sum = 0;

        while (0 != status)
        {        
            status = pthread_create(&thread[i], NULL, SumOfDividorsInRange, &wrapper[i]);
        }
    }
    
    for (i = 0; i < NUM_OF_THREADS; ++i)
    {
        pthread_join(thread[i], NULL);
        total_sum += wrapper[i].sum;
    }
    
    printf("sum of dividers: %ld\n", total_sum);

    return 0;
}
