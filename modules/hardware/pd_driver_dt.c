#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/printk.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/of.h>

#include "bone/pd.h"

#define MAX_DEVICES	2

static int test_probe(struct platform_device *);
static int test_remove(struct platform_device *);

struct device_private_data {
	struct platform_data data;
	void *buffer;
	dev_t dev_num;
	struct cdev cdev;
};

struct driver_private_data {
	int total_devices;
	dev_t base_number;
	struct class *class;
	struct device *device;
};

struct device_conf {
	int config_item1;
	int config_item2;
};

static struct device_conf dummy_conf[] = {
	{
		.config_item1 = 1,
		.config_item2 = 1,
	}, {
		.config_item1 = 2,
		.config_item2 = 2,
	}, {
		.config_item1 = 3,
		.config_item2 = 3,
	}, {
		.config_item1 = 4,
		.config_item2 = 4,
	},
};

static struct driver_private_data driver_private_data = {
	.total_devices = 2,
};

static struct platform_driver test_driver = {
	.probe = test_probe,
	.remove = test_remove,
	.driver = {
		.name = "test",
		.of_match_table = (const struct of_device_id []) {
		{
			.compatible = "test-A1x",
			.data = (void *) 0,
		}, {
			.compatible = "test-B1x",
			.data = (void *) 1,
		}, {
			.compatible = "test-C1x",
			.data = (void *) 2,
		}, {
			.compatible = "test-D1x",
			.data = (void *) 2,
		}, __END_OF_LIST
	},
	},
};

static struct file_operations fops = {
	.owner = THIS_MODULE,
};

static struct platform_data *get_data_from_dt(struct device *dev)
{
	int res;
	struct platform_data *data;

	if (!dev->of_node)
		return NULL;

	data = devm_kzalloc(dev, sizeof(*data), GFP_KERNEL);
	if (!data) {
		dev_info(dev, "No memory...");
		return ERR_PTR(-ENOMEM);
	}

	res = of_property_read_string(dev->of_node, "pskrgag,serial", &data->serial_number);
	if (res) {
		dev_info(dev, "EINVAL LOL");
		return ERR_PTR(-EINVAL);
	}

	res = of_property_read_u32_index(dev->of_node, "pskrgag,size", 0, &data->size);
	if (res) {
		dev_info(dev, "EINVAL LOL 1");
		return ERR_PTR(-EINVAL);
	}



	return data;
}

static int test_probe(struct platform_device *pd)
{
	struct device_private_data *dev_priv_data;
	struct platform_data *dev_data = dev_get_platdata(&pd->dev);
	int res;
	struct device *device;

	pr_err("Probe dummy shit = %d %d\n", dummy_conf[pd->id_entry->driver_data].config_item1,
					     dummy_conf[pd->id_entry->driver_data].config_item2);

	if (!dev_data) {
		dev_data = get_data_from_dt(&pd->dev);
		if (!dev_data) {
			pr_err("Can't find device data\n");
			return -1;
		}
	}

	dev_priv_data = devm_kmalloc(&pd->dev, sizeof(*dev_priv_data), GFP_KERNEL);
	if (!dev_priv_data) {
		pr_err("No memory 1\n");
		return -ENOMEM;
	}

	memcpy(&dev_priv_data->data, dev_data, sizeof(*dev_data));

	dev_priv_data->buffer = devm_kmalloc(&pd->dev, dev_data->size, GFP_KERNEL);
	if (!dev_priv_data->buffer) {
		pr_err("no memore 2\n");
		return -ENOMEM;
	}

	dev_priv_data->dev_num = driver_private_data.base_number + pd->id;

	cdev_init(&dev_priv_data->cdev, &fops);
	dev_priv_data->cdev.owner = THIS_MODULE;
	res = cdev_add(&dev_priv_data->cdev, dev_priv_data->dev_num, 1);
	if (res < 0) {
		pr_err("Cdev add failed\n");
		return -EINVAL;
	}

	device = device_create(driver_private_data.class, NULL, dev_priv_data->dev_num,
			       NULL, "test-%d", pd->id);
	if (IS_ERR(device)) {
		pr_err("Failed to create device file\n");
		res = -EINVAL;
		goto dev_err;
	}

	pd->dev.driver_data = dev_priv_data;

	pr_err("Probe is completed\n");
	return 0;

dev_err:
	cdev_del(&dev_priv_data->cdev);
	return res;

	pr_err("Device detected\n");
	return 0;
}

static int test_remove(struct platform_device *pd)
{
#if 0
	struct device_private_data *dev_priv_data = pd->dev.driver_data;

	device_destroy(driver_private_data.class, dev_priv_data->dev_num);
	cdev_del(&dev_priv_data->cdev);
#endif
	pr_err("Remove is completed\n");
	return 0;
}

static int __init test_init(void)
{
	int res;

	res = alloc_chrdev_region(&driver_private_data.base_number, 0, MAX_DEVICES, "test");
	if (res < 0) {
		pr_err("Alloc chrdev region failed\n");
		return res;
	}

	driver_private_data.class = class_create(THIS_MODULE, "test_driver");
	if (IS_ERR(driver_private_data.class)) {
		pr_err("Class creation failed\n");
		goto class_err;
	}

	res = platform_driver_register(&test_driver);
	if (res < 0) {
		pr_err("Driver registration failed\n");
		goto plat_err;
	}

	return 0;

plat_err:
	class_destroy(driver_private_data.class);
class_err:
	unregister_chrdev_region(driver_private_data.base_number, MAX_DEVICES);
	return -1;
}

static void __exit test_exit(void)
{
	platform_driver_unregister(&test_driver);
	unregister_chrdev_region(driver_private_data.base_number, MAX_DEVICES);
	class_destroy(driver_private_data.class);
}

MODULE_LICENSE("GPL");
module_init(test_init);
module_exit(test_exit);
