#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main()
{
	printf("hello world from process ID %ld\n", (long)getpid());
	exit(0);
}
