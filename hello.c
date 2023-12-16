#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("Anastasia Pavlush <n.pavlush16@gmail.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static uint counter = 1;
int i;

module_param(counter, uint, 0444);
MODULE_PARM_DESC(counter, "mycounter");

struct my_struct {
 struct list_head tlist;
 ktime_t time;
};

static LIST_HEAD(my_list_head);
static int __init hello_init(void)
{

 if (counter == 0)
  printk(KERN_WARNING "The parameter = 0\n");
 else if (counter >= 5 && counter <= 10)
  printk(KERN_WARNING "The parameter between 5 and 10\n");
 else if (counter > 10) {
  printk(KERN_WARNING "The parameter > 10)\n");
  return -EINVAL;
 }

 for (i = 0; i < counter; i++) {
  struct my_struct *ms = NULL;

  ms = kmalloc(sizeof(struct my_struct), GFP_KERNEL);
  ms->time = ktime_get();
  list_add_tail(&ms->tlist, &my_list_head);

  printk(KERN_INFO "Hello, world!\n");
 }
 return 0;
}
static void __exit hello_exit(void)
{
 int count = 1;
 struct my_struct *t, *tmp;

 list_for_each_entry_safe(t, tmp, &my_list_head, tlist) {
  pr_info("line: %d; Time = %lld\n", count++, ktime_to_ns(t->time));
  list_del(&t->tlist);
  kfree(t);
 }
 printk(KERN_INFO "Module exit\n");
}
module_init(hello_init);
module_exit(hello_exit);
