#include <linux/module.h>
#include <linux/errno.h>
#include <linux/fs.h> // Can remove this
#include <linux/workqueue.h> // we use workqueues

MODULE_AUTHOR("Praveen Kumar Pendyala");
MODULE_DESCRIPTION("Lab 3 Solution");
MODULE_LICENSE("GPL");

#define WORK_QUEUE "consumer_work_queue"

static struct workqueue_struct *wq;
static struct delayed_work c_task;

static void consume_item(void *param){
	printk("Consumed item\n");
	queue_delayed_work(wq, &c_task, 2*HZ);
}

// Declare a delayed work
static DECLARE_DELAYED_WORK(c_task, consume_item);

// initialize module (executed when using insmod)
static int __init consumer_init(void)
{
	// Allocate a work queue - this gives context to run our tasks in.
	wq = alloc_workqueue(WORK_QUEUE, WQ_UNBOUND, 1);

	// Declare a delayed work scheduler - helps in scheduling task later.
	//DECLARE_DELAYED_WORK(c_dwork, consume_item);

	// Queue the delayed work into our work queue
	queue_delayed_work(wq, &c_task, 2*HZ);

	return 0;	
}

// cleanup module (executed when using rmmod)
static void __exit consumer_cleanup(void)
{
	// Cancel the delayed work
	cancel_delayed_work(&c_task);

	// Destroy the workqueue
	destroy_workqueue(wq);
}

module_init(consumer_init);
module_exit(consumer_cleanup);
