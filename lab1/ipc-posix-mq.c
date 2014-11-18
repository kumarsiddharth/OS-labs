#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_BUFFER 1024
#define SAMPLE_TEXT "YOLO!"
#define QNAME "/yummy_quuee"

main(){
	pid_t childid;
	int childec;
	mqd_t mqd;
	struct mq_attr attr;
	ssize_t bytes_read = -1;
	char buffer[MAX_BUFFER + 1];
	
	// Set attributes
	attr.mq_flags = 0;  
	attr.mq_maxmsg = 10;  
	attr.mq_msgsize = 33;  
	attr.mq_curmsgs = 0;  

	// Create a queue
	mqd = mq_open(QNAME, O_CREAT | O_RDONLY, 0644, &attr);

	printf("mqd is : %d \n", mqd);
	// Attempt to fork
	childid = fork();
	
	// Parent execution
	if(childid > 0){
		printf("I'm parent. pid : %d\n", getpid());
		
		// Open queue with READ access
		mqd = mq_open(QNAME, O_RDONLY);

		printf("mqdp is : %d \n", mqd);		

		// Read from queue
		bytes_read = mq_receive(mqd, buffer, MAX_BUFFER, NULL);
		if(bytes_read == -1)
			printf("Errno is : %d\n", errno);		

		// Print the read data
		printf("Data received : %d, %s \n", (int) bytes_read, buffer);

		// Wait for child
		wait(&childec);
		printf("Waited for child, exit code : %d\n", childec);
	}

	// Child execution
	else if(childid == 0){
		printf("I'm child. pid : %d\n", getpid());
		
		// Open queue with WRITE access
		mqd = mq_open(QNAME, O_WRONLY);
		printf("mqdc is : %d \n", mqd);
		// Write to queue
		bytes_read = mq_send(mqd, SAMPLE_TEXT, sizeof(SAMPLE_TEXT), 1);
		printf("Send status: %d \n", bytes_read);
	}

	// Fork failed so, terminate
	else{
		printf("Failed to fork. Error! \n");
		perror("fork");
		exit(0);
	}

}


