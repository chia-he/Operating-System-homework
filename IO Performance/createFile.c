#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdint.h>
#define M 1<<20

int main(){

	int32_t fd;
	int32_t ret;
	int8_t *newData = valloc(sizeof(char) * 100 * M);

	fd = open("data", O_CREAT | O_RDWR );
	if(fd == -1){
		printf("error.\n");
		return 0;
	}
	ret = write(fd, newData, 100 * M);
	if(ret == -1){
		printf("error2.\n");
		return 0;
	}
	free(newData);
	close(fd);

	return 0;
}
