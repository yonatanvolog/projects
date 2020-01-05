/**********************************************************************
 *	This code is representing my tests of Heap 			              *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Chen Rubin	 	  							     	  *
 *	Date: 31.12.19													  *
 **********************************************************************/

#include <stdio.h>	/*printf*/
#include <stdlib.h>	/*malloc ,free*/
#include <assert.h>	/*assert*/

#include "heap.h"
#include "../../yoni_utils.h"

#define PTR_SIZE (sizeof(int *))
/*Uncomment the line below in order to activate white-box tests*/
#define WHITE_BOX_TEST
static int CompFunc(const void *new_data, 
                    const void *src_data,
				    void *compare_param);
static int IsMatchFunc(const void *new_data, const void *src_data);
						   
int HeapCreateTest();
int HeapDestroyTest();
int HeapPushTest();
int HeapPopTest();
int HeapRemoveTest();

static void PrintArr(int **arr, size_t arr_size);


#ifdef WHITE_BOX_TEST
struct heap
{
	d_vector_t *vector;
	comparison_t comparison_func;
	void *param;
	
};

/*maybe*/
struct d_vector_t
{
	size_t capacity;
	size_t element_size;
	size_t size;
	void *head;
};
#endif

int main()
{
	RUN_SEQUENCE("HeapCreateTest", HeapCreateTest());
	RUN_SEQUENCE("HeapDestroyTest", HeapDestroyTest());
	RUN_SEQUENCE("HeapPushTest", HeapPushTest());
	RUN_SEQUENCE("HeapPopTest", HeapPopTest());
	RUN_SEQUENCE("HeapRemoveTest", HeapRemoveTest());
	
	
	return 0;
}

int HeapCreateTest()
{
    heap_t *new_heap = HeapCreate(CompFunc, NULL);
    assert(new_heap);
	printf("----Empty Test----\n");

    HeapDestroy(new_heap);
	
	return 0;
}

int HeapDestroyTest()
{
    heap_t *new_heap = HeapCreate(CompFunc, NULL);
    assert(new_heap);
	printf("----Check Valgrind----\n");

    HeapDestroy(new_heap);
	
	return 0;
}

int HeapPushTest()
{
    int push1 = 5;
    int push2 = 6;
    int push3 = 1;
    int push4 = 100;
    int push5 = 98;
    int push6 = 20;
    int push7 = 5;
    int push8 = 1;
    int push9 = 2;
   
    heap_t *heap = NULL;
   
    heap = HeapCreate(CompFunc, NULL);
    assert(heap);
    
    HeapPush(heap, &push1);
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
    RUN_TEST("push1", 5 == *(int *)HeapPeek(heap));

    HeapPush(heap, &push2);
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
    RUN_TEST("push2", 6 == *(int *)HeapPeek(heap));

    HeapPush(heap, &push3);
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
    RUN_TEST("push3", 6 == *(int *)HeapPeek(heap));

    HeapPush(heap, &push4);
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
    RUN_TEST("push4", 100 == *(int *)HeapPeek(heap));

    HeapPush(heap, &push5);
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
    RUN_TEST("push5", 100 == *(int *)HeapPeek(heap));

    HeapPush(heap, &push6);
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
    RUN_TEST("push6", 100 == *(int *)HeapPeek(heap));

    HeapPush(heap, &push7);
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
    RUN_TEST("push7", 100 == *(int *)HeapPeek(heap));

    HeapPush(heap, &push8);
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
    RUN_TEST("push8", 100 == *(int *)HeapPeek(heap));
    
	HeapPush(heap, &push9);
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
	RUN_TEST("push9", 100 == *(int *)HeapPeek(heap));
    
#ifdef WHITE_BOX_TEST
	RUN_TEST("wb push1", 100 == **(int **)((char *)heap->vector->head + 0*PTR_SIZE));
	RUN_TEST("wb push2", 98 == **(int **)((char *)heap->vector->head + 1*PTR_SIZE));
	RUN_TEST("wb push3", 20 == **(int **)((char *)heap->vector->head + 2*PTR_SIZE));
	RUN_TEST("wb push4", 5 == **(int **)((char *)heap->vector->head + 3*PTR_SIZE));
	RUN_TEST("wb push5", 6 == **(int **)((char *)heap->vector->head + 4*PTR_SIZE));
	RUN_TEST("wb push6", 1 == **(int **)((char *)heap->vector->head + 5*PTR_SIZE));
	RUN_TEST("wb push7", 5 == **(int **)((char *)heap->vector->head + 6*PTR_SIZE));
	RUN_TEST("wb push8", 1 == **(int **)((char *)heap->vector->head + 7*PTR_SIZE));
	RUN_TEST("wb push9", 2 == **(int **)((char *)heap->vector->head + 8*PTR_SIZE));
#endif

    
    HeapDestroy(heap);
	
	return 0;
}

int HeapPopTest()
{
    int push1 = 5;
    int push2 = 6;
    int push3 = 1;
    int push4 = 100;
    int push5 = 98;
    int push6 = 20;
    int push7 = 5;
    int push8 = 1;
    int push9 = 2;
   
    heap_t *heap = NULL;
   
    heap = HeapCreate(CompFunc, NULL);
    assert(heap);
    
    HeapPush(heap, &push1);
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
    HeapPush(heap, &push2);
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
    
    HeapPush(heap, &push3);
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
    
    HeapPush(heap, &push4);
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
    
    HeapPush(heap, &push5);
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
    
    HeapPush(heap, &push6);
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
    
    HeapPush(heap, &push7);
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
    
    HeapPush(heap, &push8);
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
    
    HeapPush(heap, &push9);
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
    
    RUN_TEST("before popping", 100 == *(int *)HeapPeek(heap));
    
    HeapPop(heap);
    RUN_TEST("pop1", 98 == *(int *)HeapPeek(heap));
    RUN_TEST("size1", 8 == HeapSize(heap));
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
        
    HeapPop(heap);
    RUN_TEST("pop2", 20 == *(int *)HeapPeek(heap));
    RUN_TEST("size2", 7 == HeapSize(heap));
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
        
    HeapPop(heap);
    RUN_TEST("pop3", 6 == *(int *)HeapPeek(heap));
    RUN_TEST("size3", 6 == HeapSize(heap));
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
        
    HeapPop(heap);
    RUN_TEST("pop4", 5 == *(int *)HeapPeek(heap));
    RUN_TEST("size4", 5 == HeapSize(heap));
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
        
    HeapPop(heap);
    RUN_TEST("pop5", 5 == *(int *)HeapPeek(heap));
    RUN_TEST("size5", 4 == HeapSize(heap));
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
        
    HeapPop(heap);
    RUN_TEST("pop6", 2 == *(int *)HeapPeek(heap));
    RUN_TEST("size6", 3 == HeapSize(heap));
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
        
    HeapPop(heap);
    RUN_TEST("pop7", 1 == *(int *)HeapPeek(heap));
    RUN_TEST("size7", 2 == HeapSize(heap));
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
        
    HeapPop(heap);
    RUN_TEST("pop8", 1 == *(int *)HeapPeek(heap));
    RUN_TEST("size8", 1 == HeapSize(heap));
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
    
    HeapPop(heap);
    RUN_TEST("poped last element", 1 == HeapIsEmpty(heap));
    
    HeapDestroy(heap);
	
	return 0;
}

int HeapRemoveTest()
{
    int push1 = 5;
    int push2 = 6;
    int push3 = 1;
    int push4 = 100;
    int push5 = 98;
    int push6 = 20;
    int push7 = 5;
    int push8 = 1;
    int push9 = 2;
   
    heap_t *heap = NULL;
   
    heap = HeapCreate(CompFunc, NULL);
    assert(heap);
    
    HeapPush(heap, &push1);
    HeapPush(heap, &push2);
    HeapPush(heap, &push3);
    HeapPush(heap, &push4);
    HeapPush(heap, &push5);
    HeapPush(heap, &push6);
    HeapPush(heap, &push7);
    HeapPush(heap, &push8);
    HeapPush(heap, &push9);

    RUN_TEST("before removing", 100 == *(int *)HeapPeek(heap));
    
    HeapRemove(heap, IsMatchFunc, &push4);
    RUN_TEST("remove1", 98 == *(int *)HeapPeek(heap));
    RUN_TEST("size1", 8 == HeapSize(heap));
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
        
    HeapRemove(heap, IsMatchFunc, &push6);
    RUN_TEST("remove2", 98 == *(int *)HeapPeek(heap));
    RUN_TEST("size2", 7 == HeapSize(heap));
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
        
    HeapRemove(heap, IsMatchFunc, &push7);
    RUN_TEST("remove3", 98 == *(int *)HeapPeek(heap));
    RUN_TEST("size3", 6 == HeapSize(heap));
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
        
    HeapRemove(heap, IsMatchFunc, &push5);
    RUN_TEST("remove4", 6 == *(int *)HeapPeek(heap));
    RUN_TEST("size4", 5 == HeapSize(heap));
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
        
    HeapRemove(heap, IsMatchFunc, &push2);
    RUN_TEST("remove5", 5 == *(int *)HeapPeek(heap));
    RUN_TEST("size5", 4 == HeapSize(heap));
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
        
    HeapRemove(heap, IsMatchFunc, &push1);
    RUN_TEST("remove6", 2 == *(int *)HeapPeek(heap));
    RUN_TEST("size6", 3 == HeapSize(heap));
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
        
    HeapRemove(heap, IsMatchFunc, &push9);
    RUN_TEST("remove7", 1 == *(int *)HeapPeek(heap));
    RUN_TEST("size7", 2 == HeapSize(heap));
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
        
    HeapRemove(heap, IsMatchFunc, &push8);
    RUN_TEST("remove8", 1 == *(int *)HeapPeek(heap));
    RUN_TEST("size8", 1 == HeapSize(heap));
#ifdef WHITE_BOX_TEST
    PrintArr(heap->vector->head, HeapSize(heap));
#endif
    
    HeapRemove(heap, IsMatchFunc, &push3);
    RUN_TEST("removed last element", 1 == HeapIsEmpty(heap));
    
    HeapDestroy(heap);
	
	return 0;
}

static void PrintArr(int **arr, size_t arr_size)
{
    size_t i = 0;
    
    printf("\n{");
    for (i = 0; i < arr_size; ++i)
    {
        printf("%d, ", **(int **)(arr + i));
    }
    printf("END}\n");
}


/*
returns (1) if new_data is with higher priority,
higher value is higher priority
*/
static int CompFunc(const void *new_data, 
                    const void *src_data,
					void *compare_param)
{
    UNUSE(compare_param);
    
    if (*(int *)new_data > *(int *)src_data)
    {
        return (1);
    }
    else if (*(int *)src_data > *(int *)new_data)
    {
        return (-1);
    }
    else
    {
        return (0);
    }
}

static int IsMatchFunc(const void *new_data, const void *src_data)
{
	return (*(int *)new_data == *(int *)src_data);
}
