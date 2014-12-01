#include <linux/module.h>	/* Specifically, a module */
#include <linux/kernel.h>	/* We're doing kernel work */
#include <linux/proc_fs.h>	/* Necessary because we use the proc fs */
#include <asm/uaccess.h>	/* for copy_from_user */
#include <linux/errno.h>	/* Error numbers and errno */
#include <linux/time.h>		/* for kernel time functions */
#include <linux/slab.h>		/* For kmalloc */

#define FIFO_FILE "FIFO"

MODULE_AUTHOR("Praveen Kumar Pendyala");
MODULE_DESCRIPTION("Lab 2 Solution FIFO device");

// Read clock call
static ssize_t fifo_read(struct file *file, char *user_buf, size_t count,
						loff_t *ppos)
{
	return 0;
}

// Write clock call - unsupported
static ssize_t fifo_write( struct file *file, const char *buf, size_t count,
						 loff_t *ppos )			
{
	printk("FIFO write called\n");
	return 0;
}

// Open device call
static int fifo_open(struct inode * inode, struct file * file)
{
	printk("FIFO open called\n");
	return 0;	
}


// Release clock for future operations
static int fifo_release(struct inode * inode, struct file * file)
{
	printk("FIFO release called\n");
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
	proc_create(FIFO_FILE, 0, NULL, &fifo_fops);
	return 0;
}

// Remove clock device on rmmod
static void __exit fifo_device_cleanup(void)
{
	remove_proc_entry(FIFO_FILE, NULL);
}

module_init(fifo_device_init);
module_exit(fifo_device_cleanup);
