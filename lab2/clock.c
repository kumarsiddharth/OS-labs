#include <linux/module.h>	/* Specifically, a module */
#include <linux/kernel.h>	/* We're doing kernel work */
#include <linux/proc_fs.h>	/* Necessary because we use the proc fs */
#include <asm/uaccess.h>	/* for copy_from_user */
#include <linux/errno.h>	/* Error numbers and errno */
#include <linux/time.h>		/* for kernel time functions */
#include <linux/slab.h>		/* For kmalloc */

#define CDEV_FILE_NAME "clock"
#define YEAR_LEN 4
#define MON_DAY_LEN 2
#define SEP_LEN 1
#define HR_MIN_SEC_LEN 2 
#define TS_LEN (YEAR_LEN + 2*MON_DAY_LEN + 3*HR_MIN_SEC_LEN + 5*SEP_LEN + 4)

MODULE_AUTHOR("Praveen Kumar Pendyala");
MODULE_DESCRIPTION("Lab 2 Solution Clock driver");
MODULE_LICENSE("GPL");

// To trigger end of file while reading
int eof = 0;

/*********************************************
 For Epoch to datetime conversion
 Conversion routine credits: Stack overflow
********************************************/
struct date_time_t {
	int second;
	int minute;
	int hour;
	int day;
	int month;
	int year;
};

static unsigned short days[4][12] =
	{
	    {   0,  31,  60,  91, 121, 152, 182, 213, 244, 274, 305, 335},
	    { 366, 397, 425, 456, 486, 517, 547, 578, 609, 639, 670, 700},
	    { 731, 762, 790, 821, 851, 882, 912, 943, 974,1004,1035,1065},
	    {1096,1127,1155,1186,1216,1247,1277,1308,1339,1369,1400,1430},
	};

// Epoch to datetime conversion
static void epoch_to_date_time(struct date_time_t* date_time, unsigned int epoch)
{
	date_time->second = epoch%60; epoch /= 60;
	date_time->minute = epoch%60; epoch /= 60;
	date_time->hour   = epoch%24; epoch /= 24;

	unsigned int years = epoch/(365*4+1)*4; epoch %= 365*4+1;

	unsigned int year;
	for (year=3; year>0; year--)
	{
		if (epoch >= days[year][0])
			break;
	}

	unsigned int month;
	for (month=11; month>0; month--){
		if (epoch >= days[year][month])
			break;
	}

	date_time->year  = years+year;
	date_time->month = month+1;
	date_time->day   = epoch-days[year][month]+1;
}

/**********************************************
 End of Epoch to datetime conversion
**********************************************/

// Read clock call
static ssize_t clock_read(struct file *file, char *user_buf, size_t count,
						loff_t *ppos)
{
	struct timeval *tv;
	struct date_time_t *dt;
	char *ts;
	
	// Signal eof if previous read was done.
	if(eof == 1)
		return 0;
	
	tv = kmalloc(sizeof(*tv), GFP_KERNEL);
	dt = kmalloc(sizeof(*dt), GFP_KERNEL);
	ts = kmalloc(sizeof(*ts) * TS_LEN, GFP_KERNEL);
	
	// Get current time
	do_gettimeofday(tv);
	epoch_to_date_time(dt, tv->tv_sec);
	
	// Format the time. +1 for hours is to get to EST time zone
	sprintf(ts, "%d-%d-%d %02d:%02d:%02d\n\0", dt->day, dt->month, 
		dt->year + 1970, dt->hour + 1, dt->minute, dt->second);
	
	// Copy to userspace
	copy_to_user(user_buf, ts, TS_LEN);
	
	// Set eof to 1 for next read's
	eof = 1;
	
	// Format time
	return TS_LEN;
}

// Write clock call - unsupported
static ssize_t clock_write( struct file *file, const char *buf, size_t count,
						 loff_t *ppos )			
{
	printk("Clock write operation not supported\n");
	return -EPERM;
}

// Open device call
static int clock_open(struct inode * inode, struct file * file)
{
	printk("Clock file opened!\n");
	eof = 0;
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
