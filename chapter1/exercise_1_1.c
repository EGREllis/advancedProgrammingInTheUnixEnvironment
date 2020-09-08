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
	size_t space = (size_t)(strlen(dirp->d_name) + strlen(prefix) + 1 + strlen(FILE_SEPARATOR));
	char *name = calloc(sizeof(char), space);
	strcpy(name, prefix);
	strcpy(name+strlen(prefix), FILE_SEPARATOR);
	strcpy(name+strlen(prefix)+strlen(FILE_SEPARATOR), dirp->d_name);
	return name;
}

void recursiveFileVisitor(char *directory) {
	DIR *dp;
	struct dirent *dirp;
	int pid, status;
	FILE *log = fopen("child.log", "ab");

	dp = opendir(directory);
	while ((dirp = readdir(dp)) != NULL) {
		if (strcmp(".", dirp->d_name) == 0 || strcmp("..", dirp->d_name) == 0) {
			continue;
		}

		char *myDirName = makeMyDirectoryName(dirp, directory);
		fprintf(log, "Visited %s\n", myDirName);

		fflush(log);
		if ((pid = fork()) < 0) {
			fprintf(stderr, "Fork error\n");
			fclose(log);
			exit(1);
		} else if (pid == 0) {
			// Child
			recursiveFileVisitor(myDirName);
			fclose(log);
			exit(0);
		} else {
			// Parent
			if ((pid = waitpid(pid, &status, 0)) == 0) {
				fprintf(log, "Waitpid error!\n");
				fclose(log);
				exit(2);
			} else if (status != 0) {
				fprintf(log, "Waited for child process %d status %d\n", pid, status);
			}
		}
	}
	closedir(dp);
}

int main() {
	// Problem:
	//	1) Prove that . and .. are different directories in every directory aside from /
	// Strategy:
	//	1) Use recursive depth first search
	//	2) pwd
	//	3) Research different methods of IPC

	recursiveFileVisitor(ROOT_DIR);
}
