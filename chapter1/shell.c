#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAXLINE 80

void err_sys(char *message) {
	fprintf(stderr, "%s\n", message);
	fflush(stderr);
	exit(1);
}

void err_ret(char *template, char *message) {
	fprintf(stderr, template, message);
	fflush(stderr);
	exit(2);
}

int
main()
{
	char buf[MAXLINE];
	pid_t pid;
	int status;

	printf("%% ");	// print prompt, first pecentage to escape
	while (fgets(buf, MAXLINE, stdin) != NULL) {
		if (buf[strlen(buf) - 1] == '\n') {
			buf[strlen(buf) - 1] = '\0';	// replace new line with null
		}
		if ((pid = fork()) < 0) {
			err_sys("fork error");
		} else if (pid == 0) {
			execlp(buf, buf, (char *)0);
			err_ret("couldn't execute: %s\n", buf);
			exit(127);
		}

		// parent
		if ((pid = waitpid(pid, &status, 0)) < 0) {
			err_sys("waitpid error");
		}
		printf("%% ");
	}
}
