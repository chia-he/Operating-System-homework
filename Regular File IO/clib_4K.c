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

int main(){

	int32_t i;
	FILE *fp;
	int8_t *buffer = valloc(sizeof(char));
	int64_t exTime;
	struct timeval timeStart;
	struct timeval timeEnd;

	srand(time(NULL));
	
	gettimeofday(&timeStart, NULL);

	fp = fopen("data", "wt+");	
	
	for(i = 0; i < 1000; ++i){
		fwrite(buffer, sizeof(char), 4096, fp);
	}

	fclose(fp);
	free(buffer);

	gettimeofday(&timeEnd, NULL);
	
	exTime = 1000000 * (timeEnd.tv_sec - timeStart.tv_sec) 
			+ (timeEnd.tv_usec - timeStart.tv_usec);
	printf("C library write 4K-byte exTime = %ld us\n", exTime);

	return 0;
}
