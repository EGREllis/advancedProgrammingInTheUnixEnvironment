#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
	fprintf(stderr, "EACCES: %s\n", strerror(EACCES));
	errno = ENOENT;
	perror(argv[0]);
	exit(0);
}
