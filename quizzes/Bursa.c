#include <stdio.h>	/*printf*/

/*	This function recieves a pointer to an array and it's size. 
 *	Prints out the best profit and buy/sell indices.
 */
void Bursa (int *arr, int arr_size);

int main()
{
	int arr[] = {6, 12, 3, 5, 1, 4, 9, 2};
	/*int arr[] = {6, 2001, 0, 5, 1, 2000, 9, 2, 200};*/
	Bursa(arr, (sizeof(arr)/sizeof(int)));
	
	return 0;
}

void Bursa (int *arr, int arr_size)
{
	int buy = 0, sell = 0, profit = 0;
	int buy_r = 0, sell_r = 0;
	int i = 0;
	
	for (; i < arr_size; ++i)
	{
		if (arr[i] - arr[buy_r] > profit)
		{
			sell_r = i;
			
			buy = buy_r;
			sell = sell_r;
			profit = arr[sell] - arr[buy];
		}
		else if (arr[i] < arr[buy_r])
		{
			sell_r = i;
			buy_r = i;
		}
	}
	printf("Buy index: %d,    Sell index: %d,    Profit: %d\n", buy, sell, profit);
}

