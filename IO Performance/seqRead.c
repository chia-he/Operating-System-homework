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

	int32_t fd;
	int32_t ret;
	int32_t curr;
	int8_t *buffer = valloc(sizeof(char) * 100 * M);
	int64_t exTime;
	struct timeval timeStart;
	struct timeval timeEnd;

	fd = open("data", O_RDONLY);
	if(fd == -1){
		printf("open error1.\n");
		return 0;
	}
	curr = lseek(fd, 0, SEEK_SET);
	if(curr == -1){
		printf("offset set error.\n");
		return;
	}
	gettimeofday(&timeStart, NULL);
	ret = read(fd, buffer, 100 * M);
	gettimeofday(&timeEnd, NULL);
	if(ret == -1){
		printf("file read error.\n");
		return 0;
	}
	free(buffer);
	close(fd);

	exTime = 1000000 * (timeEnd.tv_sec - timeStart.tv_sec) 
			+ (timeEnd.tv_usec - timeStart.tv_usec);
	printf("seqRead exTime = %ld us\n", exTime);

	return 0;
}
