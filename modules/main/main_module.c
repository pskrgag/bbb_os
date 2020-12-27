#include <linux/module.h>

#include <bone/system.h>

static int __init main_module_init(void)
{
	bone_init_system();

	return 0;
}

static void __exit main_module_exit(void)
{

}

module_init(main_module_init);
module_exit(main_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("pskrgag");
