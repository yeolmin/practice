#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>

static unsigned int irq = 190;
static void *dev;

irqreturn_t my_irq_handler(int irq, void *id)
{
	printk("my_irq_handler(%d, %p)\n", irq, id );
	return IRQ_HANDLED;
}

int my_init(void)           
{                               
	int ret;
	unsigned long flags=IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING;
	printk("my_init()\n");
	ret = request_irq(irq, my_irq_handler, flags, "MY INT", dev);
	return 0;
}

void my_exit(void)
{
	printk("my_exit()\n");
	free_irq(irq,dev);
}

MODULE_LICENSE("GPL");
module_init(my_init);
module_exit(my_exit);
