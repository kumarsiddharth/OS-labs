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
#define QNAME "/yummypipe"

#define err_check(status, msg)\
	if(!(status))\
		fprintf(stderr, "Error %s. Error code : %d \n", msg, errno);\


main(){
	pid_t childid;
	int childec;
	mqd_t mqd;
	struct mq_attr attr;
	ssize_t bytes_read;
	char buffer[MAX_BUFFER + 1];
	
	// Set attributes
	attr.mq_flags = 0;  
	attr.mq_maxmsg = 10;  
	attr.mq_msgsize = MAX_BUFFER;  
	attr.mq_curmsgs = 0;  

	// Create a queue
	mqd = mq_open(QNAME, O_CREAT, 0644, &attr);
	err_check(0 <= mqd, "creating queue");
	
	// Attempt to fork
	childid = fork();
	
	// Parent execution
	if(childid > 0){
		printf("I'm parent. pid : %d\n", getpid());
		
		// Open queue with READ access
		mqd = mq_open(QNAME, O_RDONLY);
		err_check(0 <= mqd, "opening queue");
		
		// Read from queue
		err_check(0 <= mq_receive(mqd, buffer, MAX_BUFFER, NULL), "reading queue");	

		// Print the read data
		printf("Data received : %s \n", buffer);

		// Wait for child
		wait(&childec);
		printf("Waited for child, exit code : %d\n", childec);
	}

	// Child execution
	else if(childid == 0){
		printf("I'm child. pid : %d\n", getpid());
		
		// Open queue with WRITE access
		mqd = mq_open(QNAME, O_WRONLY);
		err_check(0 <= mqd, "opening queue");
		
		// Write to queue
		err_check(0 <= mq_send(mqd, SAMPLE_TEXT, sizeof(SAMPLE_TEXT), 1), "writing queue");
	}

	// Fork failed so, terminate
	else{
		printf("Failed to fork. Error code : %d \n", errno);
	}

}

