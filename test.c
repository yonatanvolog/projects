#include <stdio.h>


int main()
{
	size_t a = 0;
	size_t k = 5;
	int b = 0;	
	a = ~a;
	printf("%lu\n", a);
	b = a - 1;	
	printf("%d\n", b);
	return 0;
}


