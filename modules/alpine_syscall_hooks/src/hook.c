#include "hook.h"
#include "../../core/kernel.h"
#include "../../core/echo.h"

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/unistd.h>
#include <linux/time.h>
#include <asm/uaccess.h>
#include <linux/kallsyms.h>
#include <linux/slab.h>
#include <linux/stat.h>

#include <linux/sched.h>
#include <linux/fcntl.h>
#include <linux/file.h>
#include <linux/uio.h>
#include <linux/export.h>
#include <linux/syscalls.h>
#include <linux/pagemap.h>
#include <linux/splice.h>
#include <linux/compat.h>
/*
#include <linux/sched/xacct.h>
#include <linux/fsnotify.h>
#include <linux/security.h>
#include <linux/mount.h>
#include <linux/fs.h>
*/
static unsigned long original_syscall_table[512];

typedef 
ssize_t (*ksys_read_func)(unsigned int fd, char __user* buf, size_t count);

ssize_t alpine_ksys_read(unsigned int fd, char __user* buf, size_t count){
    echo("alpine_ksys_read.\n");
    ksys_read_func real_read = original_syscall_table[__NR_read];
    return real_read(fd, buf,count);
}

int install_hooks(void) {
    unsigned long* sys_call_table = (unsigned long*)kallsyms_lookup_name("sys_call_table");
    echo("sys_call_table address %p\n", sys_call_table);
    make_vm_rw(sys_call_table);
    original_syscall_table[__NR_read] = sys_call_table[__NR_read];
    sys_call_table[__NR_read] = alpine_ksys_read;
    make_vm_ro(sys_call_table);
    return 0;
}
void uninstall_hooks(void){
    unsigned long* sys_call_table = (unsigned long*)kallsyms_lookup_name("sys_call_table");
    echo("sys_call_table address %p\n", sys_call_table);
    make_vm_rw(sys_call_table);
    sys_call_table[__NR_read] = original_syscall_table[__NR_read];
    make_vm_ro(sys_call_table);
}
