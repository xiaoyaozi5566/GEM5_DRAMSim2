#include <assert.h>
#include <pthread.h>
#include <stdlib.h>

#include <stdio.h>

volatile static int shared = 0;
//static pthread_rwlock_t lock;
	
void* attacker(void* arg)
{
	int i, local;
	int id = (int)arg;
	for (i = 0; i < 1000; i++){
		//pthread_rwlock_wrlock(&lock);
		local = shared;
		shared = shared + 1;
		//pthread_rwlock_unlock(&lock);
		//printf("Thread %d: shared = %d\n", id, shared);
	}
	
	printf("Thread %d: local = %d\n", id, local);
	
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
	
	//pthread_rwlock_init(&lock, NULL);
	
	//pthread_create(&pth, &attr, attacker, (void*)1);
	
	int i, local;
	for (i = 0; i < 10; i++){
		//pthread_rwlock_wrlock(&lock);
		local = shared;
		shared = shared - 1;
		//pthread_rwlock_unlock(&lock);
		printf("Thread 1: shared = %d\n", shared);
	}
	
	printf("Thread 1: local = %d\n", local);
	
	//pthread_join(pth, NULL);
	
	printf("Victim PASSED :)\n");
}