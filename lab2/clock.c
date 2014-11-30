#include <linux/module.h>
#include <linux/errno.h>
#include <linux/fs.h>

MODULE_AUTHOR("Praveen Kumar Pendyala");
MODULE_DESCRIPTION("Lab 2 Solution Clock driver");
MODULE_LICENSE("GPL");

// Read clock call
static ssize_t clock_read(struct file *file, char *buf, size_t count,
						loff_t *ppos)
{

}

// Write clock call - unsupported
static ssize_t clock_write( struct file *file, const char *buf, size_t count,
						 loff_t *ppos )			
{
	printk("Operation not supported");
}

// Open device call
static int clock_open(struct inode * inode, struct file * file)
{

}

// Release clock for future operations
static int clock_release(struct inode * inode, struct file * file)
{

}

// clocks' file operations structure
static struct file_operations gen_module_fops = {
	.owner =	THIS_MODULE,
	.read =		clock_read,
	.write =	clock_write,
	.open =		clock_open,
	.release =	clock_release,
};

// Initialize clock device on insmod
static int __init clock_device_init(void)
{

}

// Remove clock device on rmmod
static void __exit clock_device_cleanup(void)
{

}

module_init(clock_device_init);
module_exit(clock_device_cleanup);
