#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define BUFFER_SIZE 100
#define READ_END 0
#define WRITE_END 1

int ipc_test()
{
	pid_t pid;
	char parent_msg[BUFFER_SIZE] = "Greetings from parent!";
	char child_msg[BUFFER_SIZE] = "Hello from child!";
	char read_parent_msg[BUFFER_SIZE] ;
	char read_child_msg[BUFFER_SIZE] ;
	int fd1[2];//parent -> child pipe
	int fd2[2];//child -> parent pipd
	

	
	if(pipe(fd1) == -1 || pipe(fd2) ==-1 )//pipe failed
	{
		fprintf(stderr, "Pipe failed");
		return 1;
	}
	pid= fork();//create child process
	
	
	if(pid < 0)//fork faild
	{
		fprintf(stderr, "Fork Failed");
		return 1;
	}
	else if(pid > 0)//parent process
	{
		close(fd1[READ_END]);
		close(fd2[WRITE_END]);
		write(fd1[WRITE_END], parent_msg, strlen(parent_msg));
		close(fd1[WRITE_END]);
		read(fd2[READ_END], read_child_msg, BUFFER_SIZE);
		printf("Child message : %s\n", read_child_msg);
		close(fd2[READ_END]);
		waitpid(pid,NULL,0);
	}
	else//child process
	{
		close(fd1[WRITE_END]);
		close(fd2[READ_END]);
		read(fd1[READ_END], read_parent_msg, BUFFER_SIZE);
		printf("Parent message : %s\n", read_parent_msg);
		close(fd1[READ_END]);
		write(fd2[WRITE_END], child_msg, strlen(child_msg));
		close(fd2[WRITE_END]);
	}
	return 0;
}
	
