//
//  cbz.c
//  cbz
//
//  Created by Trevor Downs on 2013-08-24.
//
//

#include "cbz.h"
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

int main(int argc, const char * argv[])
{
	unsigned int i;
	struct stat file_stats;
	pid_t starting_pid, fork_pid, wait_pid;
	int execresult;
	char archive[512];
	
	starting_pid = getpid();
	//process flags
	
	if(argc == 1)
	{
		printf("USAGE: cbz comicdir1 [comicdir2\u2026]\n");
		exit(1);
	}
	
	for (i = 1; i < argc; i++)
	{
		errno = 0;
		
		if (lstat(argv[i], &file_stats) != 0) {
			perror(argv[i]);
			continue;
		}

		if(!(file_stats.st_mode & S_IFDIR))
		{
			printf("\"%s\" is not a directory, skipping.\n", argv[i]);
			continue;
		}
				
		errno = 0;
		fork_pid = fork();
		
		switch (fork_pid) {
			case 0: //exec to zip
				strcpy(archive, argv[i]);
				strcat(archive, ".cbz");
				errno = 0;
				
				execlp("zip", "-mrT9", archive, argv[i], "-x \"*.DS_Store\" \"*[Tt]humbs.db\"");
				printf("errno is %d\n", errno);
				perror("execlp");
				exit(1);
				break;
				
			case -1:
				perror("fork() failed");
				exit(2);
				break;
				
			default:
				wait_pid = wait(&execresult);
				printf("Child %d finished %ssucessfully.\n", fork_pid, WIFEXITED(execresult) ? "" : "un");
				break;
		}
	}

	return 0;
}

