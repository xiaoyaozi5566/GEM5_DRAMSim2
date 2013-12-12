#include <stdio.h>
#include <stdlib.h>
int main() {
	int i, j;
	int * buffer;
	buffer = (int *) malloc(sizeof(int)*2000);
	for (i = 0; i < 2000; i++) 
	{
		for (j = 0; j < 2; j++)
			buffer[i] = i;
	}
	return 0;
}
