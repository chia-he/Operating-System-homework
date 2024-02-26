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
	int32_t fd;
	int8_t *buffer = valloc(sizeof(char) * 8192);
	int64_t exTime;
	struct timeval timeStart;
	struct timeval timeEnd;

	srand(time(NULL));
	
	gettimeofday(&timeStart, NULL);

	fd = open("data", O_CREAT | O_WRONLY, 4777);	
	
	for(i = 0; i < 500; ++i){
		write(fd, buffer, sizeof(char) * 8192);
	}

	close(fd);
	free(buffer);

	gettimeofday(&timeEnd, NULL);

	exTime = 1000000 * (timeEnd.tv_sec - timeStart.tv_sec) 
			+ (timeEnd.tv_usec - timeStart.tv_usec);
	printf("direct write 8K-byte exTime = %ld us\n", exTime);

	return 0;
}
