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
	int32_t fd1, fd2;
	int8_t *buffer = valloc(sizeof(char) * 4);
	int64_t exTime;
	struct timeval timeStart;
	struct timeval timeEnd;

	srand(time(NULL));

	fd1 = open("data", O_RDONLY);
	fd2 = open("file", O_CREAT | O_WRONLY);
	
	gettimeofday(&timeStart, NULL);
	
	for(i = 0; i < (int)(25*M); ++i){
		read(fd1, buffer, sizeof(char) * 4);
		write(fd2, buffer, sizeof(char) * 4);
	}

	gettimeofday(&timeEnd, NULL);	
	
	close(fd2);
	close(fd1);
	free(buffer);	

	exTime = 1000000 * (timeEnd.tv_sec - timeStart.tv_sec) 
			+ (timeEnd.tv_usec - timeStart.tv_usec);
	printf("System call 4B-sized I/O exTime = %ld us\n", exTime);

	return 0;
}
