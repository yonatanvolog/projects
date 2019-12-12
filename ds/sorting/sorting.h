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

int RadixSort(int *arr, size_t arr_size);

