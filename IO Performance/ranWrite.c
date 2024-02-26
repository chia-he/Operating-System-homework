#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#define __USE_GNU 1
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdint.h>
#include <time.h>
#define M 1<<20
#define K 1<<10


int main(){

	int i, n, offset;
	int32_t fd;
	int32_t ret;
	int32_t curr;
	int8_t *buffer = valloc(sizeof(char) * 4 * K);
	int64_t exTime;
	struct timeval timeStart;
	struct timeval timeEnd;

	srand(time(NULL));
	
	fd = open("data", O_WRONLY);
	if(fd == -1){
		printf("open error2-2.\n");
		return 0;
	}
	n = (100 * M) / (4 * K);
	gettimeofday(&timeStart, NULL);
	for(i = 0; i < 50000; i++){
		offset = (rand() % n) * (4 * K);
		curr = lseek(fd, offset, SEEK_SET);
		ret = write(fd, buffer, 4 * K);
	}
	gettimeofday(&timeEnd, NULL);
	if(ret == -1){
		printf("file write error.\n");
		return 0;
	}
	free(buffer);
	close(fd);

	exTime = 1000000 * (timeEnd.tv_sec - timeStart.tv_sec) 
			+ (timeEnd.tv_usec - timeStart.tv_usec);
	printf("ranWrite exTime = %ld us\n", exTime);

	return 0;
}
