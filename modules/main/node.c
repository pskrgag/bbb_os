#include <linux/export.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#include <bone/node.h>
#include <bone/system.h>
#include <bone/api/olaf_api.h>
#include <bone/ioctl.h>

long bone_ioctl(struct file *, unsigned int, unsigned long);

static dev_t dev_num;
static struct cdev *cdev;

static struct file_operations bone_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = bone_ioctl
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
	cdev->owner = THIS_MODULE;

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

long bone_ioctl(struct file *fl, unsigned int cmd, unsigned long arg)
{
	struct bone_request req;
	ssize_t res;
	void *arg_kern;

	pr_err("Im in ioclt\n");

	/* this copy is needed to verify user's pointer */
	if (copy_from_user(&req, (void *) arg, sizeof(req)))
		return -EINVAL;

	arg_kern = kmalloc(OLAF_COMMAND_ARGS_SIZE(req.code), GFP_KERNEL);
	if (!arg)
		return -ENOMEM;

	/* this copy is needed to verify user's pointer */
	if (OLAF_COMMAND_PERMS(req.code) & OLAF_WRITE) {
		if (copy_from_user(arg_kern, req.arg,
		    OLAF_COMMAND_ARGS_SIZE(req.code))) {
			res = -EINVAL;
			goto end;
		}
	}

	switch(cmd) {
	case BONE_DEV:
		res = bone_disp(req.code, arg_kern);
		break;
	default:
		pr_err("Wrong command");
		res = -EINVAL;
	}

	if (res < 0)
		goto end;

	if (OLAF_COMMAND_PERMS(req.code) & OLAF_READ) {
		if (copy_to_user(req.arg, arg_kern,
		    OLAF_COMMAND_ARGS_SIZE(req.code))) {
			res = -EINVAL;
			goto end;
		}
	}

	pr_err("Out ioclt\n");
end:
	kfree(arg_kern);
	return res;
}
