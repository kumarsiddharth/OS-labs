#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_BUFFER 1024
#define SAMPLE_TEXT "YOLO!"

main(){
	pid_t childid;
	int childec;
	int fd[2];
	char buffer[MAX_BUFFER];
	
	// Create a pipe
	pipe(fd);

	// Attempt to fork
	childid = fork();
	
	// Parent execution
	if(childid > 0){
		printf("I'm parent. pid : %d\n", getpid());
		
		// Close writing to pipe
		close(fd[1]);
		
		// Read from pipe
		read(fd[0], buffer, MAX_BUFFER);
		
		// Print the data read from pipe
		printf("Read from pipe: %s\n", buffer);

		// Wait for child
		wait(&childec);
		printf("Waited for child, exit code : %d\n", childec);
	}

	// Child execution
	else if(childid == 0){
		printf("I'm child. pid : %d\n", getpid());
		
		// Close reading from pipe
		close(fd[0]);

		// Write to pipe
		write(fd[1], SAMPLE_TEXT, sizeof(SAMPLE_TEXT));
	}

	// Fork failed so, terminate
	else{
		printf("Failed to fork. Error! \n");
		perror("fork");
		exit(0);
	}

}


