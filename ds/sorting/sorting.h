/**********************************************************************
 *							sorting algorithms		     	          *
 *																	  *
 *							  Date: 10.12.19						  *
 **********************************************************************/

typedef int (*compar_func_t)(const void *, const void *);
typedef int (*is_before_t)(const void *new_data, 
						   const void *src_data,
						   void *compare_param);

void BubbleSort(int *arr, size_t size);
void InsertionSort(int *arr, size_t size);
void SelectionSort(int *arr, size_t size);

int CountingSort(const int *arr,
				 size_t arr_size,
				 int min_val,
				 int max_val,
				 int *result);

int RadixSort(int *arr, size_t arr_size);


int MergeSort(int *arr, size_t arr_size);
void HeapSort(int *arr, size_t arr_size, size_t elem_size, 
			  			     is_before_t func, void *param);
void QuickSort(void *arr, size_t arr_size, size_t elem_size, compar_func_t func);


