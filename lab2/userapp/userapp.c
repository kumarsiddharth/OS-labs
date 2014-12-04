#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define FIFO0	"/dev/fifo0"
#define FIFO1	"/dev/fifo1"
#define FIFO2	"/dev/fifo2"
#define FIFO3	"/dev/fifo3"

#define TEST_SHORT_TEXT "Yo LKM!"
#define TEST_SHORT_TEXT "Yolo. Let's test this with a long message :D"

void send_cmd(int fd, int cmd)
{
	int retval = 0;
	int i = 0;
	char buffer[BUFFER_SIZE];
	
	for(i=0;i<BUFFER_SIZE;i++){
		buffer[i] = 0x6a;
	}
	
	retval = write(fd, buffer, BUFFER_SIZE);
	if (retval < 0)
		fprintf(stderr, "an error occured: %d\n", retval);
}

int main (){	
	int f0, f1, f2, f3;
	
	clock_t start, end;
	double cpu_time = 0;
	double data_sent;
	
	fd = open(dev, O_RDWR);
	if (fd == -1)
	{
		perror("open");
		exit(1);
	}
	
	fprintf(stderr, "The device is: %s\n", DEFAULT_DEVICE);

	start = clock();

	for(i=0; i<BANDWIDTH_TEST_CYCLES; i++){
		send_cmd(fd, cmd);
		end = clock();
		cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC;
	}
	
	data_sent = BUFFER_SIZE * BANDWIDTH_TEST_CYCLES;
	
	fprintf(stdout, "Sent %lf MB in %lf seconds\n", data_sent/(1024*1024), cpu_time);

	close(fd);
}
