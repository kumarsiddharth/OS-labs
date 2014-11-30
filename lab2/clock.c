#include <linux/module.h>	/* Specifically, a module */
#include <linux/kernel.h>	/* We're doing kernel work */
#include <linux/proc_fs.h>	/* Necessary because we use the proc fs */
#include <asm/uaccess.h>	/* for copy_from_user */
#include <linux/errno.h>	/* Error numbers and errno */
#include <linux/time.h>		/* for kernel time functions */
#include <linux/slab.h>		/* For kmalloc */

#define CDEV_FILE_NAME "clock"
#define CDEV_MAJOR 2
#define CDEV_MINOR 3

MODULE_AUTHOR("Praveen Kumar Pendyala");
MODULE_DESCRIPTION("Lab 2 Solution Clock driver");
MODULE_LICENSE("GPL");

// Read clock call
static ssize_t clock_read(struct file *file, char *buf, size_t count,
						loff_t *ppos)
{
	struct timeval *tv;
	tv = kmalloc(sizeof(*tv), GFP_KERNEL);
	
	// Get current time
	do_gettimeofday(tv);
	
	printk("The time is: %ld\n", tv->tv_sec);
	
	// Format time
	return 0;
}

// Write clock call - unsupported
static ssize_t clock_write( struct file *file, const char *buf, size_t count,
						 loff_t *ppos )			
{
	printk("Operation not supported");
	return -EPERM;
}

// Open device call
static int clock_open(struct inode * inode, struct file * file)
{
	return 0;	
}


// Release clock for future operations
static int clock_release(struct inode * inode, struct file * file)
{
	return 0;
}

// clocks' file operations structure

static struct file_operations clock_fops = {
	.owner =	THIS_MODULE,
	.read =		clock_read,
	.write =	clock_write,
	.open =		clock_open,
	.release =	clock_release,
};

// Initialize clock device on insmod
static int __init clock_device_init(void)
{	
	proc_create(CDEV_FILE_NAME, 0, NULL, &clock_fops);
	return 0;
}

// Remove clock device on rmmod
static void __exit clock_device_cleanup(void)
{
	remove_proc_entry(CDEV_FILE_NAME, NULL);
}

module_init(clock_device_init);
module_exit(clock_device_cleanup);
