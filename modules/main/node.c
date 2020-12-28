#include <linux/export.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>

#include <bone/node.h>

static dev_t dev_num;
static struct cdev *cdev;
static struct file_operations bone_fops = {
	.owner = THIS_MODULE,

};

static struct class *class;

int __init bone_create_node(void)
{
	int res;

	res = alloc_chrdev_region(&dev_num, 0, 1, "bone");
	if (res < 0) {
		pr_err("Failed to allocate region");
		return res;
	}

	cdev = cdev_alloc();
	if (!cdev) {
		pr_err("Failed to allocate node");
		goto cdev_err;
	}

	cdev_init(cdev, &bone_fops);

	res = cdev_add(cdev, dev_num, 1);
	if (res < 0) {
		pr_err("Failed to add chardev");
		goto class_err;
	}

	class = class_create(THIS_MODULE, "bone");
	if (!class) {
		pr_err("Failed to create class");
		goto class_err;
	}

	device_create(class, NULL, dev_num, NULL, "bone");

	return 0;

class_err:
	cdev_del(cdev);
cdev_err:
	unregister_chrdev_region(dev_num, 1);

	return -1;
}

void __exit bone_destroy_node(void)
{
	device_destroy(class, dev_num);
	class_destroy(class);
	cdev_del(cdev);
	unregister_chrdev_region(dev_num, 1);
}
