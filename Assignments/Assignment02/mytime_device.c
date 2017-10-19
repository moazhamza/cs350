#include <linux/init.h>
#include <linux/module.h>

MODULE_LICESNSE("DUAL BSD/GPL");

static struct miscdevice mytime_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "mytime device",
	.fops = &mytime_fops
};

static struct file_operations mytime_fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_close,

}
