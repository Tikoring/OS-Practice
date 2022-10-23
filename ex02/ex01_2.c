#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int main (void) {
	FILE *fp;
	if ((fp = fopen ("unix.txt", "r")) == NULL) {
		printf ("errono = %d", errno);
		exit (1);
	}
	fclose (fp);
	return 0;
}