#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

// Create an inode (file-ish) for the device with
// $ mknod /dev/foobar c <MAJOR_NUMBER> 0

#define DEVICE_NAME "foobar"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name Here");
MODULE_DESCRIPTION("Creates a most useful device");
MODULE_VERSION("1.0.0");

static int dev_open(struct inode* inode_ptr, struct file* file_ptr) {
    printk(KERN_INFO "Device opened\n");
    return 0;
}

static int dev_release(struct inode* inode_ptr, struct file* file_ptr) {
    printk(KERN_INFO "Device released\n");
    return 0;
}

static ssize_t dev_read(struct file* file_ptr, char* buffer, size_t buffer_len, loff_t* offset) {
    printk(KERN_INFO "Device read\n");

    const char * message = "fooooooooo...";
    int len = strlen(message);

    int res = copy_to_user(buffer, message, len);

    return res == 0 ? len : -EFAULT;
}

static ssize_t dev_write(struct file* file_ptr, const char* buffer, size_t buffer_len, loff_t* offset) {
    printk(KERN_INFO "Device written\n");
    return -EFAULT;;
}



static struct file_operations fops = {
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
    // There are a lot more field in this struct actually...
};

static int major;



static int __init lkm_init(void) {

    major = register_chrdev(0, DEVICE_NAME, &fops);

    if (major < 0) {
        printk(KERN_INFO "Failed to register device\n");
        return major;
    }

    printk(KERN_INFO "Module loaded: %d\n", major);
    return 0;
}

static void __exit lkm_exit(void) {
    unregister_chrdev(major, DEVICE_NAME);

    printk(KERN_INFO "Module unloaded\n");
}

module_init(lkm_init);
module_exit(lkm_exit);
