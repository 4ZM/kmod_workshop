## Tiny Kernel Module Intro

This is a _very_ brief introduction to kernel modules.

Most operating systems execute code in (at least) two context; the
kernel and user space. These contexts are some times reffered to as
ring 0 (kernel) and ring 3 (user space). Most CPU's have hardware
support for togging between the two contexts and only allow some
operations to be performed while in the privileged context. All
control of hardware (network card, file system, usb, etc.) needs to be
handled by the kernel.

If we want to access hardware there are a few options. We could write
user space programs that request the kernel to do stuff for us using
system calls or special devices. We could also "become the kernel" by
compiling code into the kernel itself. The most convenient way to get
into the kernel is writing a kernel module that can be loaded, by the
kernel, at runtime. This way we don't need to re-compile the kernel
all the time.

In general, if you can solve a problem in user space. But some times
that isn't possible.


## Computer setup for the workshop

It is recommended to do kernel development in a virtual machine since
the whole OS could (will) crash if you make a programming error.

Install prereqs inside the VM:

`apt install build-essential linux-headers-$(uname -r)`

Then mount a shared dir from the host with the source code:

```
sudo mount -t vboxsf shared ~/shared
```

Work on the source code from the host and only compile and test the
module in the VM.


## Where to look for information

General information about writing drivers for different types of hardware:
https://www.kernel.org/doc/html/v4.13/driver-api/index.html

Kernel API: https://www.kernel.org/doc/htmldocs/kernel-api/
Kernel Build System: https://www.kernel.org/doc/Documentation/kbuild/makefiles.txt

To look up defines or function declarations etc, the kernel header files are available here:
`/usr/src/linux-headers-$(uname -r)/include/linux/module.h`

For reading about syscalls, man (section 2) is actually pretty useful:
Eg. `man 2 open`


## Working with kernel modules

Build the kernel module with one of the provided Makefiles. It uses the kernel build system.

List loaded modules with `lsmod`
Load a module with `insmod my-module.ko`
Unload a module with `rmmod my-module`
Get info on a module with `modinfo my-module`

Use `tail -f /var/log/kern.log` or `dmesg -w` to see log print outs from the kernel. Useful for debugging.

## Kernel workshop part #1 : Skeletons

Take a look at the `hello_world` kernel module.

Read through the code, discuss and try to understand what all parts of the code does.

Build it, load it and monitor the kernel log.

Next, take a look at the `hello_params` module.

Read through the code, discuss and try to understand what all parts of the code does.

Feel free to change the code to see what the result is when playing around with it.


## Kernel workshop part #2 : Devices

One way to let user space programs access restricted functionality
like hardware is to create a device that is presented as a file,
typically under `/dev/`.

Take a look at what files there are in `/dev` and try to find out what
some of them are.

The `device` directory contains a kernel module that creates a new
device. To create an entry under `/dev` you run `mknod /dev/foobar c
<MAJOR_NUMBER> 0`.

Discuss the code and try to understand what it's doing together in the
group. Look up anything that you don't understand.

Modify the device so that you can write a number to the device and get
that ammount of "oooo"'s when reading from it. Or change it is some
other way that you think would be fun.

In this blog post the author describes how to get around having to
manually `mknod` and also how to make the module thread safe using
mutexes. Browse through it and try to implement these improvements.
http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/


## Kernel Workshop part #3 : Syscalls

As mentioned before, one way for user space programs to access
restricted functionality is using system calls. Let's explore that a
bit.

Take a look at `readfile/readfile.c`. Build it and try it out.

What does the program do?

Run the program under `strace`

`$ strace ./readfile <somefile> > /dev/null`

This shows all system calls that are issued. Try to correlate it with the code.

Pick another random program and run it with strace. Pick 5 syscalls
and read up on what they do (man section 2)

## Kernel Workshop part #4 : rootkit

Back in the days, hackers could try to use the kernel to hide what was
going by having the kernel present a fake view of the
system. Malicious code running in the kernel is some times known as a
rootkit. It could hook syscalls for listing files to hide a file that
was actually there or hide processes so that ps wouldn't show them.

In `hook_syscall` you can experiment with syscall hijacking yourself.

Build and try it out. Don't forget to put the secret file in it's
right spot (with 666 permissions) before you start. Or not. Try what
happens it isn't there as well.

Once you understand what is going on, try changing the program a bit.

Finally, find another fun syscall to hook and try it out. There are
plenty to choose from (`man 2 syscalls`).
