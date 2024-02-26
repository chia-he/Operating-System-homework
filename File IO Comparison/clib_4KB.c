#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#define __USE_GNU 1
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#define M 1<<20
#define K 1<<10

int main(){

	int32_t i;
	FILE *fp1, *fp2;
	int8_t *buffer = valloc(sizeof(char) * 4 * K);
	int64_t exTime;
	struct timeval timeStart;
	struct timeval timeEnd;

	srand(time(NULL));

	fp1 = fopen("data", "r");
	fp2 = fopen("file", "w+");
	
	gettimeofday(&timeStart, NULL);
	
	for(i = 0; i < (int)(25*K); ++i){
		fread(buffer, sizeof(char), 4 * K, fp1);
		fwrite(buffer, sizeof(char), 4 * K, fp2);
	}

	gettimeofday(&timeEnd, NULL);	
	
	fclose(fp2);
	fclose(fp1);
	free(buffer);	

	exTime = 1000000 * (timeEnd.tv_sec - timeStart.tv_sec) 
			+ (timeEnd.tv_usec - timeStart.tv_usec);
	printf("C library 4KB-sized I/O exTime = %ld us\n", exTime);

	return 0;
}
