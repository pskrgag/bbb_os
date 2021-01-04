#include <linux/export.h>
#include <linux/module.h>

#include <bone/system.h>
#include <bone/node.h>

int __init bone_init_system(void)
{
	bone_create_node();

	return 0;
}

void __exit bone_clean_system(void)
{
	bone_destroy_node();
}

static int bone_get_device_info(olaf_code_t code, struct olaf_device_info* arg)
{
	strncpy(arg->name, "BeagleBone Black by pskrgag", OLAF_MAX_DEVICE_NAME);

	return sizeof(*arg);
}

int bone_disp(olaf_code_t code, void *arg)
{
	switch (code) {
	case OLAF_GET_DEVICE_INFO:
		return bone_get_device_info(code, arg);
	default:
		return -EINVAL;
	}

	return 0;
}
