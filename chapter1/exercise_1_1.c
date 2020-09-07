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

char *makeMyDirectoryName(struct dirent *dirp, char *prefix) {
	size_t space = (size_t)(strlen(dirp->d_name) + strlen(prefix) + 1);
	char *name = calloc(sizeof(char), space);
	strcpy(name, FILE_SEPARATOR);
	strcpy(name+strlen(FILE_SEPARATOR), dirp->d_name);
	return name;
}

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

	dp = opendir(ROOT_DIR);
        while ((dirp = readdir(dp)) != NULL) {
		char* prefix = ROOT_DIR;
		if (strcmp(".", dirp->d_name) == 0 || strcmp("..", dirp->d_name) == 0) {
			prefix = "";
		}
		fprintf(stdout, "%s%s\n", prefix, dirp->d_name);

		if ((pid = fork()) < 0) {
			fprintf(stderr, "Fork error\n");
			exit(1);
		} else if (pid == 0) {
			FILE *log = fopen("child.log", "ab");

			char *myDirName = makeMyDirectoryName(dirp, prefix);
			fprintf(log, "Pid: %d\tPath: %s\n", getpid(), myDirName);

			free(myDirName);
			fclose(log);
			exit(0);
		} else {
			if ((pid = waitpid(pid, &status, 0)) < 0) {
				fprintf(stderr, "Waitpid error!\n");
			} else if (errno != 0) {
				fprintf(stderr, "Waited for child process %d status %d\n\terrorno: %d\terror: %s\n", pid, status, errno, strerror(errno));
			}
		}
        }
	closedir(dp);
}
