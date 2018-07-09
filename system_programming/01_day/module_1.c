#include <linux/module.h>
#include <linux/kernel.h>

int init_module(void)	//정의로 이동 : g + ]
{
	printk("init_module()\n");

	return 0;
}
void cleanup_module(void)
{
	printk("cleanup_module()\n");
}
