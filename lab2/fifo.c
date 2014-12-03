#include <linux/module.h>	/* Specifically, a module */
#include <linux/kernel.h>	/* We're doing kernel work */
#include <linux/proc_fs.h>	/* Necessary because we use the proc fs */
#include <asm/uaccess.h>	/* for copy_from_user */
#include <linux/errno.h>	/* Error numbers and errno */
#include <linux/time.h>		/* for kernel time functions */
#include <linux/slab.h>		/* For kmalloc */

#define FIFO_NAME "fifo"
#define FIFO_MAJOR_NUM 240
#define BUFFER_MAX_SIZE 10
#define FIFO_MAX_COUNT 4

char buffer[FIFO_MAX_COUNT/2][BUFFER_MAX_SIZE];
int running[FIFO_MAX_COUNT];
int curpos[FIFO_MAX_COUNT/2];

MODULE_AUTHOR("Praveen Kumar Pendyala");
MODULE_DESCRIPTION("Lab 2 Solution FIFO device");
MODULE_LICENSE("GPL");

// Read clock call
static ssize_t fifo_read(struct file *file, char *user_buf, size_t count,
						loff_t *ppos)
{
	// Get minor number to see if operation is supported
	int minor = MINOR(file->f_dentry->d_inode->i_rdev);
	int bytes_read = 0;
	int fifo;
	
	// Only odd number FIFOs are read ends
	if(minor%2 == 0){
		printk(KERN_ALERT "Write only fifo end. Read not permitted\n");
		return -EACCES;
	}
	
	// Identify the fifo number
	fifo = (minor-1)%2;
	
	printk(KERN_INFO "Curpos is %d\n", curpos[fifo]);
	
	// Check if fifo is empty
	if(curpos[fifo] == 0){
		printk(KERN_INFO "Fifo %d is empty\n", fifo);
		return 0;
	}
	
	// Fifo has data. Read it.
	else{
		bytes_read = curpos[fifo];
		printk(KERN_INFO "Buffer is %s \n", buffer[fifo][0]);
		printk(KERN_INFO "Buffer iss %s \n", buffer[fifo]);
		copy_to_user(user_buf, buffer[fifo], sizeof(int)*curpos[fifo]);
		curpos[fifo] = 0;
		return bytes_read;
	}
}

// Write to fifo call
static ssize_t fifo_write( struct file *file, const char *buf, size_t count,
						 loff_t *ppos )			
{
	// Get minor number to see if operation is supported
	int minor = MINOR(file->f_dentry->d_inode->i_rdev);
	int bytes_written = 0;
	int fifo;
	
	// Only even number FIFOs are write ends
	if(minor%2 != 0){
		printk(KERN_ALERT "Read only fifo end. Write not permitted\n");
		return -EACCES;
	}
	
	printk(KERN_INFO "Count is %d\n", count);
	
	// Identify the fifo number
	fifo = minor%2;
	
	// Signal writer running
	running[minor] = 1;
	
	while(bytes_written != count){
	
		// Check if buffer is full
		if(curpos[fifo] != BUFFER_MAX_SIZE){
			printk(KERN_INFO "BW: %d CNT: %d\ buf %c \n", bytes_written, count, *buf);
			buffer[fifo][curpos[fifo]] = *buf++;
			curpos[fifo]++;
			bytes_written++;
		} 
		
		// Buffer full. Sleep and wait for a reader.
		else{
			printk(KERN_INFO "Writer %d sleeping\n", minor);
		}
		
	}
	
	printk("FIFO write called\n");
	return bytes_written;
}

// Open device call
static int fifo_open(struct inode * inode, struct file * file)
{
	printk("FIFO opened\n");
	return 0;	
}


// Release clock for future operations
static int fifo_release(struct inode * inode, struct file * file)
{
	printk("FIFO released\n");
	return 0;
}

// clocks' file operations structure
static struct file_operations fifo_fops = {
	.owner =	THIS_MODULE,
	.read =		fifo_read,
	.write =	fifo_write,
	.open =		fifo_open,
	.release =	fifo_release,
};

// Initialize clock device on insmod
static int __init fifo_device_init(void)
{	
	int major;
	
	// Register char driver
	major = register_chrdev(FIFO_MAJOR_NUM, FIFO_NAME, &fifo_fops);
	if(major < 0){
		printk(KERN_ALERT "Error registering char driver %d\n", major);
		return major;
	}

	return 0;
}

// Remove clock device on rmmod
static void __exit fifo_device_cleanup(void)
{
	unregister_chrdev(FIFO_MAJOR_NUM, FIFO_NAME);
}

module_init(fifo_device_init);
module_exit(fifo_device_cleanup);
