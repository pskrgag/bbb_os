#include <linux/module.h>

#include <bone/system.h>

static int __init main_module_init(void)
{
	int res;

	res = bone_init_system();
	if (res < 0)
		return -1;

	return 0;
}

static void __exit main_module_exit(void)
{
	bone_clean_system();
}

module_init(main_module_init);
module_exit(main_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("pskrgag");
