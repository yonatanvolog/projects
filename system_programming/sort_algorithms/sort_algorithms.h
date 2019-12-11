/**********************************************************************
 *							sorting algorithms		     	          *
 *																	  *
 *							  Date: 10.12.19						  *
 **********************************************************************/

void BubbleSort(int *arr, size_t size);
void InsertionSort(int *arr, size_t size);
void SelectionSort(int *arr, size_t size);

int CountingSort(const int *arr,
				 size_t arr_size,
				 int min_val,
				 int max_val,
				 int *result);


void CountingSortIMP(const void *arr_to_sort,
					 size_t element_size,
					 size_t arr_size,
					 unsigned int *histogram,
					 size_t his_size,
					 size_t (*GetBucket)(void *element, void *param),
					 void *param,
					 void *result);



