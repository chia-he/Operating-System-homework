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

	int32_t i;
	FILE *fp;
	int8_t *buffer = valloc(sizeof(char));

	fp = fopen("data", "w+");
	
	for(i = 0; i < (int)(100*M); ++i){
		fwrite(buffer, sizeof(char), 1, fp);
	}

	fclose(fp);
	free(buffer);	

	return 0;
}
