#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kallsyms.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name Here");
MODULE_DESCRIPTION("Hooking a syscall for fun and profit");
MODULE_VERSION("1.0.0");

static unsigned long ** sys_call_table;

static asmlinkage long (*original_sys_openat)(int dfd, const char __user *filename, int flags, umode_t mode);

asmlinkage long hooked_sys_openat(int dfd, const char __user *filename, int flags, umode_t mode)
{
    /* This is the custom version of the hooked openat syscall */
    long r = original_sys_openat(dfd, filename, flags, mode);
    printk(KERN_INFO "A file was opened\n");
    return r;
}

static void make_rw(unsigned long address)
{
    unsigned int level;
    pte_t *pte = lookup_address(address, &level);
    if(pte->pte &~ _PAGE_RW)
        pte->pte |= _PAGE_RW;
}

static void make_ro(unsigned long address)
{
    unsigned int level;
    pte_t *pte = lookup_address(address, &level);
    pte->pte = pte->pte &~ _PAGE_RW;
}

static int __init lkm_init(void) {
    printk(KERN_INFO "hihowaryou!\n");

    /* Find the address of the system call table using using the
     * kernel API for symbol lookup */
    sys_call_table = (void*) kallsyms_lookup_name("sys_call_table");

    /* Save original function ptr */
    original_sys_openat = (void*)sys_call_table[__NR_openat];

    /* Make page writable then update the call table */
    make_rw(sys_call_table);
    sys_call_table[__NR_openat] = (void*)hooked_sys_openat;
    make_ro(sys_call_table);

    printk(KERN_INFO "init done!\n");
    return 0;
}

static void __exit lkm_exit(void) {
    printk(KERN_INFO "byebye!\n");

    /* Reset the table by pointing to old function */
    make_rw(sys_call_table);
    sys_call_table[__NR_openat] = (void*)original_sys_openat;
    make_ro(sys_call_table);
}

module_init(lkm_init);
module_exit(lkm_exit);
