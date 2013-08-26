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
#include <sys/syslimits.h>

int main(int argc, const char * argv[])
{
	unsigned int i;
	struct stat file_stats;
	pid_t starting_pid, fork_pid, wait_pid;
	int wait_result;
	char archive[NAME_MAX + 1];
	
	starting_pid = getpid();
	THEBUG("Parent PID is %d.\n", starting_pid);
	
	if(argc == 1)
	{
		printf("USAGE: cbz comicdir1 [comicdir2\u2026]\n");
		return 0;
	}
	
	for (i = 1; i < argc; i++)
	{
		errno = 0;
		wait_result = 0;
		
		if (lstat(argv[i], &file_stats) != 0) {
			fprintf(stderr, "\a");
			perror(argv[i]);
			continue;
		}

		//if != directory,
		THEBUG("Directory test: %d\n", S_ISDIR(file_stats.st_mode));
		if(S_ISDIR(file_stats.st_mode) == 0)
		{
			fprintf(stderr, "\a\"%s\" is not a directory, skipping.\n", argv[i]);
			continue;
		}
		
		//truncates the file name if argument.cbz is bigger than NAME_MAX.
		if ((strlen(argv[i]) + strlen(FILE_EXTENSION)) <= NAME_MAX)
			strcpy(archive, argv[i]);
		else
			strncpy(archive, argv[i], NAME_MAX - strlen(FILE_EXTENSION));
		
		strcat(archive, FILE_EXTENSION);
		
		errno = 0;
		
		if ( (fork_pid = fork()) == 0)
		{
			THEBUG("%d: '%s' '%s' '%s' '%s' '%s'\n", getpid(), ZIP, ZIP_FLAGS, archive, argv[i], ZIP_EXCLUDE);
			errno = 0;
			execlp(ZIP, ZIP_FLAGS, archive, argv[i], ZIP_EXCLUDE, (char *)0);	//(char *)0 instead of NULL for compatibility. See `man execl`.
			THEBUG("%d: errno is %d\n", getpid(), errno);
			perror("execlp");
			exit(errno);
		}
		
		if (fork_pid == -1)
		{
			perror("fork() failed");
			exit(errno);
		}
		
		THEBUG("%d: fork() returned %d.\n", getpid(), fork_pid);
		
		errno = 0;
		if( (wait_pid = wait(&wait_result)) == -1 || errno != 0)
		{
			perror("wait");
			THEBUG("errno is %d, wait returned %d.\n", errno, wait_pid);
			exit(errno);
		}
		else
		{
			THEBUG("Child %d finished %ssucessfully.\n", fork_pid, WIFEXITED(wait_result) ? "" : "un");	//This probably isn't right.
		}
		
	}

	return 0;
}

