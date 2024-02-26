#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#define __USE_GNU 1
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#define M 1<<20
#define K 1<<10

int main(){

	int32_t i;
	int32_t fd1, fd2;
	char *ptr1, *ptr2;
	int32_t buffer = sizeof(char) * 4 * K;
	int64_t exTime;
	struct timeval timeStart;
	struct timeval timeEnd;

	fd1 = open("data", O_RDONLY);
	fd2 = open("file", O_CREAT | O_RDWR);
	ptr1 = mmap(0, 100 * M, PROT_READ, MAP_SHARED, fd1, 0);	
	truncate("./file", 100 * M);
	ptr2 = mmap(0, 100 * M, PROT_WRITE, MAP_SHARED, fd2, 0);
	
	gettimeofday(&timeStart, NULL);
	
	for(i = 0; i < (int)(25*K); ++i){
		memcpy(ptr2 + i * buffer, ptr1 + i * buffer, buffer);
	}

	gettimeofday(&timeEnd, NULL);	
	
	munmap(ptr2, 100 * M);
	munmap(ptr1, 100 * M);	
	close(fd2);
	close(fd1);

	exTime = 1000000 * (timeEnd.tv_sec - timeStart.tv_sec) 
			+ (timeEnd.tv_usec - timeStart.tv_usec);
	printf("Memory-mapped 4KB-sized I/O exTime = %ld us\n", exTime);

	return 0;
}
