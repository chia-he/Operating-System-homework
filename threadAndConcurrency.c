#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

#define MINPID 0
#define MAXPID 99
#define PIDSIZE MAXPID-MINPID+1
 
int *bitmap;
int testBit(int k) {
	if ( (bitmap[(k/32)] & (1<<(k%32))) )
		return 1;
	else
		return 0;
}
void printBitmap() {
	int i;
	printf("[ Bitmap ]\n");
	for( i = 0 ; i < PIDSIZE; i++) {	
		printf("Bitmap/%d/ = %d\n", i, testBit(i));
	}
}
int allocateMap() {
	bitmap = calloc( ceil(PIDSIZE/32), sizeof(int));	
	if (bitmap != NULL)
		return 1;
	else
		return -1;
}
int allocatePid() {
	int i;
	for ( i = 0; i < PIDSIZE; i++) {
		if (!testBit(i)) {
			bitmap[(i/32)] |= (1<<(i%32));
			return i;	
		}	
	}
	return -1;
}
void releasePid(int pid) {
	bitmap[(pid/32)] &= ~(1<<(pid%32));
	return;
}
void *threadLife() {
	int selfpid = allocatePid();
	printf("pid of %lu is %2d\n", pthread_self(), selfpid);
	sleep((rand()%3)+1);
	releasePid(selfpid);
	printf("pid %2d is released\n", selfpid);
	pthread_exit(NULL);
}
int main(void) {
	
	int i;
	pthread_t thread[PIDSIZE];
	srand(time(NULL));
	
	//printf("### allocating bitmap ... ###\n");
	allocateMap();

	//printf("### allocating pid ... ###\n");
	for( i = 0; i < PIDSIZE; i++) {
		pthread_create(&thread[i], NULL, threadLife, NULL);
	}

	for( i = 0; i < PIDSIZE; i++) {
		pthread_join(thread[i], NULL);
	}
	
	free(bitmap);
	return 0;
}