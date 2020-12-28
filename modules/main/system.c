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


