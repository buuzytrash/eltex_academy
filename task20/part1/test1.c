#include <linux/kernel.h>
#include <linux/module.h>

int init_module(void)
{
  pr_info("Hello, from my module!!!\n");

  return 0;
}

void cleanup_module(void) { pr_info("Goodbye, from my module!!!\n"); }

MODULE_LICENSE("GPL");