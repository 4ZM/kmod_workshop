#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name Here");
MODULE_DESCRIPTION("Hello World of kernel modules");
MODULE_VERSION("1.0.0");

static int debug=0;
module_param(debug, int, 0660);

static char *greet = "major Tom";
module_param(greet, charp, 0660);

static int __init lkm_init(void) {
    printk(KERN_INFO "Kernel space to %s!\n", greet);
    return 0;
}

static void __exit lkm_exit(void) {
    printk(KERN_INFO "Bye, bye from kernel space!\n");
}

module_init(lkm_init);
module_exit(lkm_exit);
