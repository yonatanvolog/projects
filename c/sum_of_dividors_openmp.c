/*
Best time: 125 threads

compile with: -fopenmp
*/

#include <stdio.h>
#include <omp.h>

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
    int num_of_threads = 0;
    size_t total_sum = 0;
    size_t i = 0;
    size_t num = 2500012500;
    
    wrapper_t wrapper[NUM_OF_THREADS] = {0};
    
    /*omp gets num of threads from env*/
	#pragma omp parallel
	#pragma omp for
    for (i = 0; i < NUM_OF_THREADS; ++i)
    {
        wrapper[i].num = num;
        wrapper[i].start = (num / NUM_OF_THREADS) * i + 1;
        wrapper[i].end = (num / NUM_OF_THREADS) * (i + 1);
        wrapper[i].sum = 0;
    
        num_of_threads = omp_get_num_threads();
        printf("current running thread: %d\n", omp_get_thread_num());

		SumOfDividorsInRange(&wrapper[i]);
    }
    printf("\nnum_of_threads: %d\n\n", num_of_threads);
    
    for (i = 0; i < NUM_OF_THREADS; ++i)
    {
        total_sum += wrapper[i].sum;
    }
    
    printf("sum of dividers: %ld\n", total_sum);

    return 0;
}
