#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

#define PWD_COMMAND "pwd"
#define LS_COMMAND "ls"
#define ROOT_DIR "/"
#define FILE_SEPARATOR "/"

int main() {
	// Problem:
	//	1) Prove that . and .. are different directories in every directory aside from /
	// Strategy:
	//	1) Use recursive depth first search
	//	2) pwd
	//	3) Research different methods of IPC

	struct dirent *dirp;
	DIR *dp;
	int pid, status;

	int *counter = malloc(sizeof(int));
	*counter = 0;

	dp = opendir(ROOT_DIR);
        while ((dirp = readdir(dp)) != NULL) {
		char* prefix = ROOT_DIR;
		if (strcmp(".", dirp->d_name) == 0 || strcmp("..", dirp->d_name) == 0) {
			prefix = "";
		}
                printf("%s%s\n", prefix, dirp->d_name);

		if ((pid = fork()) < 0) {
			fprintf(stderr, "Fork error\n");
			exit(1);
		} else if (pid == 0) {
			FILE *log = fopen("child.log", "ab");
			fprintf(log, "Child pid: %d\n", getpid());
			*counter = *counter + 1;
			fflush(log);

			fprintf(log, "Counter: %d\tFilename: %s\tName length: %lu\tPrefix: %s\tPrefix length: %lu\n", *counter, dirp->d_name, strlen(dirp->d_name), prefix, strlen(prefix));
			fflush(log);

			size_t space = (size_t)(strlen(dirp->d_name) + strlen(prefix) + 1);
			fprintf(log, "Allocating space %lu for path\n", space);
			fflush(log);
			char *name = calloc(sizeof(char), space);
			fprintf(log, "Allocated space for path\n");
			strcpy(FILE_SEPARATOR, name);
			fprintf(log, "Separator: %s\n", name);
			fflush(log);
			strcpy(dirp->d_name, name+strlen(FILE_SEPARATOR));
			fprintf(log, "Path: %s\n", name);

			fclose(log);
			exit(0);
		} else {
			fprintf(stderr, "Spawned child process %d\n", pid);
			if ((pid = waitpid(pid, &status, 0)) < 0) {
				fprintf(stderr, "Waitpid error!\n");
			}
			fprintf(stderr, "Waited for child process %d counter %d status %d\n\terrorno: %d\terror: %s\n", pid, *counter, status, errno, strerror(errno));
			perror("Child process returned...");
		}
        }
	closedir(dp);
}
