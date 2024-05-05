#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int duplicate()
{
	pid_t pid;
	pid = fork();
	if(pid <0)
	{
		fprintf(stderr, "Fork Failed");
		return 1;
	}
	else if(pid == 0)
	{
		printf("This is the child process. Child PID: %d\n", getpid());
		execlp("/bin/ls", "ls", NULL);

	}
	else
	{
		wait(NULL);
		printf("This is the parent process. Parent PID: %d\n", getpid());
		printf("Child Complete\n");
	}
	
	return 0;
}
		
