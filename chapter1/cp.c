#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

int
main(void) 
{
	int n;
	char buf[BUFFER_SIZE];

	while ((n = read(STDIN_FILENO, buf, BUFFER_SIZE)) > 0) {
		if (write(STDOUT_FILENO, buf, n) != n) {
			fprintf(stderr, "write error\n");
			exit(2);
		}
	}

	if (n < 0) {
		fprintf(stderr, "read error\n");
		exit(1);
	}
	exit(0);
}
