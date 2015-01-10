#include <linux/module.h>
#include <linux/errno.h>
#include <linux/fs.h> // Can remove this
#include <linux/workqueue.h> // we use workqueues

MODULE_AUTHOR("Praveen Kumar Pendyala");
MODULE_DESCRIPTION("Lab 3 Solution");
MODULE_LICENSE("GPL");

static void add_item(int item){
	printk("Item added\n");
}

static int get_item(){
	printk("Item returned");
	return 3;
}

// initialize module (executed when using insmod)
static int __init fifo_init(void)
{
	return 0;	
}

// cleanup module (executed when using rmmod)
static void __exit fifo_cleanup(void)
{

}

module_init(fifo_init);
module_exit(fifo_cleanup);
