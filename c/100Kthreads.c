/*
compile with: -pthread
*/

#include <stdio.h>
#include <pthread.h>

#define NUM (100000)

int arr[NUM] = {0};
pthread_t threads[NUM] = {0};

void *FillIndex(void *index_ptr)
{
    int index = (int)index_ptr;
    arr[index] = index;
    
    pthread_exit(NULL);
}

int main()
{
    int i = 0;
    int status = 0;
    pthread_t thread_id = 0;
    
    for (i = 0; i < NUM; ++i)
    {
        status = pthread_create(&thread_id, NULL, FillIndex, (void *)i);
        while (0 != status)
        {
            status = pthread_create(&thread_id, NULL, FillIndex, (void *)i);
        }
        
        pthread_detach(thread_id);
    }
    
    /*testing*/
    for (i = 0; i < NUM; ++i)
    {
        printf("arr[%d] = %d\n",i ,arr[i]);
        
        if (i != arr[i])
        {
            printf("error\n");
            
            return 1;
        }
    }
    printf("passed\n");

    
    return 0;
}
