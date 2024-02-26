#include <stdlib.h>
#include <stdio.h>

#define MINPID 0
#define MAXPID 5
#define PIDSIZE MAXPID-MINPID+1
 
int *bitmap;
int testBit(int k) {
	if ( (bitmap[(k/32)] & (1<<(k%32))) )
		return 1;
	else
		return 0;
}
int allocateMap() {
	bitmap = calloc( 1, sizeof(int));	
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
void printBitmap() {
	int i;
	printf("[ Bitmap ]\n");
	for( i = 0 ; i < PIDSIZE; i++) {	
		printf("Bitmap/%d/ = %d\n", i, testBit(i));
	}
}
int main(void) {
	
	int i;
	printf("### allocating bitmap ... ###\n");
	allocateMap();
	printBitmap();	

	printf("### allocating pid ... ###\n");
	for( i = 0; i < 7; i++) {
		printf("process%d pid = %d\n", i, allocatePid());
	}	
	printBitmap();

	printf("### releasing pid ... ###\n");
	for( i = 0; i < 6; i++) {
		printf("release process%d pid\n", i);
		releasePid(i);	
	} 
	printBitmap();
	
	free(bitmap);
	return 0;
}