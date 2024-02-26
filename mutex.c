#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

#define MINPID 300
#define MAXPID 399
#define PIDSIZE MAXPID-MINPID+1

pthread_mutex_t mtx;

int count;
pthread_t threadList[PIDSIZE];
int idList[PIDSIZE];
double timeList[PIDSIZE];
int *bitmap;
void *threadLife();
int testBit(int);
int allocateMap();
int allocatePid();
void releasePid(int);
void showResult();

int main(void) {

	int i;
	
	/*Initialize*/
	pthread_t thread[PIDSIZE];
	pthread_mutex_init( &mtx, NULL); 
	srand(time(NULL));
	allocateMap();
	count = 0;

	/*Creating Threads*/
	for( i = 0; i < PIDSIZE; i++) {
		pthread_create(&thread[i], NULL, threadLife, NULL);
	}

	/*Wait Till Threads Done*/
	for( i = 0; i < PIDSIZE; i++) {
		pthread_join(thread[i], NULL);
	}

	/*Show Result*/
	showResult();	
	
	/*Release Resource*/
	pthread_mutex_destroy(&mtx);
	free(bitmap);
	
	return 0;
}
void *threadLife() {
	
	double selftime = clock();
	/*Entry*/
	pthread_mutex_lock(&mtx);
	
	/**Critical Section**/
	double sleepTime = ( (3.0 - 1.0) * rand() ) / ( RAND_MAX + 1.0 ) + 1.0;
	int selfcnt = count++;
	int selfpid = allocatePid();
	threadList[selfcnt] = pthread_self();
	idList[selfcnt] = selfpid + 300;

	/*Exit*/
	pthread_mutex_unlock(&mtx);
	
	/*Sleep*/
	sleep(sleepTime);
	//sleep(rand()%3+1);

	/*Entry*/
	pthread_mutex_lock(&mtx);

	/**Critical Section**/
	releasePid(selfpid);	
	
	/*Exit*/
	pthread_mutex_unlock(&mtx);
	
	timeList[selfcnt] = (clock() - selftime)/ CLOCKS_PER_SEC;
	pthread_exit(NULL);
}

int testBit(int k) {
	if ( (bitmap[(k/32)] & (1<<(k%32))) )
		return 1;
	else
		return 0;
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

void showResult() {
	int i;
	printf("Order\tThread ID\tPID\tTime\n");
	for(i = 0; i < PIDSIZE; i++) {
		printf("%3d\t%lu\t%d\t%.4lf\n",i+1,threadList[i],idList[i],timeList[i]);
	}
	
}