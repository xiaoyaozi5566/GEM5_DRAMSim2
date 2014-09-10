#include <assert.h>
#include <pthread.h>
#include <stdlib.h>

#include <stdio.h>

volatile static int shared = 0;
	
void* victim0(void* arg)
{
	int i, local=0;
	int id = (int)arg;
	for (i = 0; i < 10000; i++){
		local = local + i;
		//printf("Thread %d: local = %d\n", id, local);
	}
	
	return NULL;
}

int main(int argc, char** argv)
{
	pthread_t pth;
	pthread_attr_t attr;
	
    int result = pthread_attr_init(&attr);
    assert(result == 0);
    result = pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    assert(result == 0);
	
	pthread_create(&pth, &attr, victim0, (void*)3);
	
	int i, local;
	for (i = 0; i < 10000; i++){
		local = shared;
		shared = shared - 1;
		//printf("Thread 2: shared = %d\n", shared);
	}
	
	printf("Thread 2: local = %d\n", local);
	
	pthread_join(pth, NULL);
	
	printf("Victim0 PASSED :)\n");
}