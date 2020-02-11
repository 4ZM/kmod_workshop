#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kallsyms.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name Here");
MODULE_DESCRIPTION("Hooking a syscall for fun and profit");
MODULE_VERSION("1.0.0");

static unsigned long ** sys_call_table;

asmlinkage long (*original_sys_openat)(struct pt_regs *);

// Since  Kernel 4.17 - parameters are passed through a pt_regs
// a lot of examples online are wrong...
//            arg1   arg2   arg3   arg4    arg5   arg6
// Register:  di     si     dx     r10     r8     r9

asmlinkage long hooked_sys_openat(struct pt_regs * regs)
{
    /* This is the custom version of the hooked openat syscall */

    const char* zneekyfile = "/dash.jpg";  // Remember 666 permissions
    char* filename = regs->si;
    int filename_len = strlen(regs->si);

    if (strcmp(zneekyfile, filename) == 0) {
        printk(KERN_INFO "Jay! The best image on the computer!\n");
    }
    else if (filename_len > 4 && strcmp(filename + filename_len - 4, ".jpg") == 0) {
        printk(KERN_INFO "Some one tried to open a b0000000ring image. Can't have that.\n");
        copy_to_user(filename, zneekyfile, strlen(zneekyfile)+1);
    }

    return original_sys_openat(regs);
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
    printk(KERN_INFO "Comence sneekinezification...\n");

    /* Find the address of the system call table using using the
     * kernel API for symbol lookup */
    sys_call_table = (void*) kallsyms_lookup_name("sys_call_table");

    /* Save original function ptr */
    original_sys_openat = (void*)sys_call_table[__NR_openat];

    /* Make page writable then update the call table */
    make_rw(sys_call_table);
    sys_call_table[__NR_openat] = (void*)hooked_sys_openat;
    make_ro(sys_call_table);

    printk(KERN_INFO "Sneekinez iz now ON!\n");
    return 0;
}

static void __exit lkm_exit(void) {
    printk(KERN_INFO "Okidokie! Turning of sneekinez!\n");

    /* Reset the table by pointing to old function */
    make_rw(sys_call_table);
    sys_call_table[__NR_openat] = (void*)original_sys_openat;
    make_ro(sys_call_table);

    printk(KERN_INFO "It's done. No more sneekinez.\n");
}

module_init(lkm_init);
module_exit(lkm_exit);
