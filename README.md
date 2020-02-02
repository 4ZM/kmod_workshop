= Tiny Kernel Module Intro =

Install prereqs `apt install build-essential linux-headers-$(uname -r)`

But it's better to get the VirtualBox image and mount a shared dir from the host.

```
sudo mount -t vboxsf shared ~/shared
```

Header files are available here:
/usr/src/linux-headers-$(uname -r)/include/linux/module.h

Install and remove the module with
insmod my_module.ko
rmmod my_module

Check the syslog for kernel kprints:
dmesg -w


Kernel API: https://www.kernel.org/doc/htmldocs/kernel-api/
Kernel Build: https://www.kernel.org/doc/Documentation/kbuild/makefiles.txt
