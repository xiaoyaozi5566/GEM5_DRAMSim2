#include <stdio.h>
#include <stdlib.h>

int main() {
	int i, j;
	int * buffer;
	buffer = (int *) malloc(sizeof(int)*1000);
	
	for (i = 0; i < 1000; i++) 
	{
		for (j = 0; j < 50; j++)
			buffer[i] = i;
	}
	
	return 0;
}