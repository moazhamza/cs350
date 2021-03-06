#include <linux/init.h>
#include <linux/module.h>
#include <linux/time.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/semaphore.h>
#include <linux/mutex.h>

MODULE_LICENSE("DUAL BSD/GPL");



static DEFINE_SEMAPHORE(entriesConsumable);
static DEFINE_SEMAPHORE(entriesEmpty);
static DEFINE_MUTEX(lock);

static int N = 100;
module_param(N, int, 0);

int buffer[1024];
int nextEntryToWrite = 0;
int nextEntryToRead = 0;

static ssize_t numpipe_read(struct file *file, char __user* out, size_t size, loff_t *off){
	int numToRead;

	if(access_ok(VERIFY_WRITE, out, size)){
		down_interruptible(&entriesConsumable);
		mutex_lock_interruptible(&lock);
		// CRITICAL SECTION
		
		numToRead = buffer[nextEntryToRead];
		nextEntryToRead++;
		if(nextEntryToRead > N) nextEntryToRead = 0;


		if(copy_to_user(out, &numToRead, size) != 0){
			printk(KERN_ALERT "numpipe_device: copy_to_user failed");
			return -EFAULT;
		}
		// END CRITICAL SECTION

		mutex_unlock(&lock);
		up(&entriesEmpty);
	}
	else{
		printk(KERN_ALERT "numpipe_device: Access was not allowed on read");
		return -EFAULT;
	}
	return size;
}

static ssize_t numpipe_write(struct file *file, const char __user* buf, size_t size, loff_t *off){
	int *numberToBeAdded = kzalloc(size, GFP_KERNEL);

	if(!numberToBeAdded){
		printk(KERN_ALERT "numpipe_device: Memory allocation for writing failed");
	       	return -EFAULT;
	}
	if(copy_from_user(numberToBeAdded, buf, size) != 0){
		printk(KERN_ALERT "numpipe_device: copy_from_user failed");
		return -EFAULT;
	}

	down_interruptible(&entriesEmpty);
	mutex_lock_interruptible(&lock);

	// CRITICAL SECTION
	
	buffer[nextEntryToWrite] = *numberToBeAdded;
	nextEntryToWrite++;
	if(nextEntryToWrite > N) nextEntryToWrite = 0;

	// END CRITICAL SECTION
	
	mutex_unlock(&lock);
	up(&entriesConsumable);

	return size;
}
static struct file_operations numpipe_fops = {
	.read = numpipe_read,
	.write = numpipe_write
};

static struct miscdevice numpipe_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "numpipe_device",
	.fops = &numpipe_fops
};


int __init numpipe_init(void){
	int err = misc_register(&numpipe_device);
	sema_init(&entriesConsumable, 0);
	sema_init(&entriesEmpty, N);
	return err;
}

void __exit my_exit(void){
	misc_deregister(&numpipe_device);
}

module_init(numpipe_init);
module_exit(my_exit);

