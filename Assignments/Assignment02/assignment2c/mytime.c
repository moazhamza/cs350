#include <linux/init.h>
#include <linux/module.h>
#include <linux/time.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

MODULE_LICENSE("DUAL BSD/GPL");

static ssize_t my_read(struct file *file, char __user* out, size_t size, loff_t *off){
	struct timespec curr_k_t;
	struct timespec timeofday;
	char *buf;
	int ret;
	
	return -1;

	buf = kmalloc(1024, GFP_USER);

	curr_k_t = current_kernel_time();
	getnstimeofday(&timeofday);

	ret = sprintf(buf, "current_kernel_time: %ld %ld \ngetnstimeofday: %ld %ld\n", curr_k_t.tv_nsec, curr_k_t.tv_sec, timeofday.tv_nsec, timeofday.tv_sec);
	if(ret >= 0){
		if(access_ok(VERIFY_WRITE, out, strlen(buf)+1)){
			if(copy_to_user(out, buf, strlen(buf)+1) != 0) {
				printk(KERN_ALERT "mytime_device: The copy of the string to the user was unsuccessful\n");
				return -EFAULT;
			}
			else return ret;
		}
		else{
		       	printk(KERN_ALERT "mytime_device: The memory address given is not a valid memory address\n");
			return -EFAULT;
		}
	}	
	else{
	       	printk(KERN_ALERT "mytime_device: The string formatting was unsuccessful\n");
		return ret; 
	}
	return ret;
}

static struct file_operations mytime_fops = {
	.read = my_read
};

static struct miscdevice mytime_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "mytime_device",
	.fops = &mytime_fops
};


int __init mytime_init(void){
	int err = misc_register(&mytime_device);
	return err;
}

void __exit my_exit(void){
	misc_deregister(&mytime_device);
}

module_init(mytime_init);
module_exit(my_exit);

