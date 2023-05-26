#include "hook.h"
#include "../../core/kmm.h"
#include "../../core/echo.h"

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/unistd.h>
#include <linux/time.h>
//#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/kallsyms.h>
#include <linux/fs.h>
#include <linux/syscalls.h>

/*
#include <asm/uaccess.h>
#include <linux/fs.h>

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/unistd.h>
#include <linux/time.h>
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
*/
/*
#include <linux/sched/xacct.h>
#include <linux/fsnotify.h>
#include <linux/security.h>
#include <linux/mount.h>

*/
typedef long unsigned int hook_func_address;

typedef ssize_t (*ksys_read_func)(unsigned int fd, char __user* buf, size_t count);

typedef struct  _hook_s {
    unsigned int index;
    hook_func_address address;
}hook_s;

static long unsigned int original_syscall_table[512];

/** __NR_read
 *
 */
ssize_t alpine_ksys_read(unsigned int fd, char __user* buf, size_t count) {
    ssize_t r;
    ksys_read_func real_read;

    real_read = (ksys_read_func)original_syscall_table[__NR_read];
    r = real_read(fd, buf, count);
    //echo("alpine_ksys_read.\n");// 调用原函数之前输出 可能引起死循环 待确定 -- fs-syscall 中输出会导致死循环
    return r;
}


int alpine_ksys_getdents(unsigned int fd,struct compat_linux_dirent __user* dirent, unsigned int count){
    return 0;
}

/** __NR_getdents64
 *
 */
int alpine_ksys_getdents64(unsigned int fd,struct linux_dirent64 __user* dirent, unsigned int count){
    return 0;
}

static hook_s alpine_syscall_hooks[] = {
    {
        .index   = __NR_read,
        .address = alpine_ksys_read,
    },
};

void make_syscall_table_rw(void){}
void make_syscall_table_ro(void){}

long unsigned int* lookup_syscall_table(void) {
    long unsigned int* table = PAGE_OFFSET;
    long unsigned int* end = VMALLOC_START < ULLONG_MAX ? VMALLOC_START : ULLONG_MAX;
    while (table != end && table + __NR_exit * sizeof(long unsigned int*) < end) {
        if (table[__NR_exit] == (long unsigned int)sys_exit) {
            echo("sys_call_table address %p\n", table);
            return table;
        }
        table += sizeof(long unsigned int*);
    }
    echo("sys_call_table address missed.\n");
    return 0;
};

inline long unsigned int hook_syscall_item(unsigned int index, long unsigned int address){
    long unsigned int* sys_call_table = lookup_syscall_table();//(long unsigned int*)kallsyms_lookup_name("sys_call_table");
    make_vm_rw((long unsigned int)sys_call_table);
    original_syscall_table[index] = sys_call_table[index];
    sys_call_table[index] = (long unsigned int)address;
    make_vm_ro((long unsigned int)sys_call_table);

    return 0;
}

int hook_syscall(hook_s* hooks,unsigned int count){
    long unsigned int* sys_call_table = lookup_syscall_table();//(long unsigned int*)kallsyms_lookup_name("sys_call_table");
    make_vm_rw((long unsigned int)sys_call_table);
    int i = 0;
    for(i ;i < (int)count ;i ++){
        original_syscall_table[hooks[i].index] = sys_call_table[hooks[i].index];
        sys_call_table[hooks[i].index] = (long unsigned int)hooks[i].address;
    }
    make_vm_ro((long unsigned int)sys_call_table);
    return i++ ;
}


static long unsigned int* sys_call_table = 0;

int install_hooks(void) {
    sys_call_table = lookup_syscall_table();// (long unsigned int*)kallsyms_lookup_name("sys_call_table");
    echo("sys_call_table address %p\n", sys_call_table);
    if(!sys_call_table) return 0;
    make_vm_rw((long unsigned int)sys_call_table);
    original_syscall_table[__NR_read] = sys_call_table[__NR_read];
    sys_call_table[__NR_read] = (long unsigned int)alpine_ksys_read;
    make_vm_ro((long unsigned int)sys_call_table);
    return 0;
}
void uninstall_hooks(void){
    if(!sys_call_table) return ;
    echo("sys_call_table address %p\n", sys_call_table);
    make_vm_rw((long unsigned int)sys_call_table);
    sys_call_table[__NR_read] = original_syscall_table[__NR_read];
    make_vm_ro((long unsigned int)sys_call_table);
}
