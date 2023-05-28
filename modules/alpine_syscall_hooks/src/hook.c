#include "hook.h"
#include "../../core/kmm.h"
#include "../../core/echo.h"

#include <linux/string.h>

#include <asm/processor.h>
#include <asm/unistd.h>
#include <asm/syscall.h>
#include <linux/mm.h>

#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/sched.h>
#include <linux/kallsyms.h>
#include <linux/dirent.h>
#include <linux/stddef.h>
#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/time.h>
#include <linux/errno.h>
#include <linux/stat.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/fsnotify.h>
#include <linux/dirent.h>
#include <linux/security.h>
#include <linux/syscalls.h>
#include <linux/compat.h>
#include <linux/uaccess.h>
#include <asm/unaligned.h>
#include <linux/fdtable.h>
#include <linux/kprobes.h>

/** NOTE
 *  sys_call_table address 18446744072432386912 /proc/kallsyms 与 通过kprobe获取的值不一致
 */

//#include <linux/unistd.h>
//#include <linux/syscalls.h>
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

/** 内核中的原子操作问题 -- 中断发生的随机性 preempt_enable preempt_disable
 * 
 */

/** 开源HOOK项目
 * https://github.com/stdhu/kernel-inline-hook 
 * 
 */

/** ftrace https://www.kernel.org/doc/html/latest/trace/ftrace-uses.html
 *         https://github.com/ilammy/ftrace-hook/blob/master/ftrace_hook.c
 *         https://blog.csdn.net/pwl999/article/details/107426138 
 */

/** kprobe https://zhuanlan.zhihu.com/p/455693301
 * 
 */

/** kallsysms_lookup_name https://zhuanlan.zhihu.com/p/518244444
 *内核版本5.7以上，无法通过kallsyms_lookup_name函数导出，我们可以使用kprobe进行导出符号表。
 */
typedef long unsigned int hook_func_address;


typedef struct _linux_dirent64 { // https://man7.org/linux/man-pages/man2/getdents.2.html
//    ino64_t        d_ino;    /* 64-bit inode number */
//    off64_t        d_off;    /* 64-bit offset to next structure */
    unsigned short d_reclen; /* Size of this dirent */
    unsigned char  d_type;   /* File type */
    char           d_name[]; /* Filename (null-terminated) */
}linux_dirent64_;


typedef ssize_t (*ksys_read_func)(unsigned int fd, char __user* buf, size_t count);
typedef long    (*ksys_getdents_func)(unsigned int fd, struct linux_dirent* dirp,unsigned int count);
typedef ssize_t (*ksys_getdents64_func)(int fd, void* dirp, size_t count); // void* dirp

typedef struct  _hook_s {
    unsigned int index;
    hook_func_address address;
}hook_s;

static long unsigned int original_syscall_table[512];
static long unsigned int* syscall_table = 0;

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


int alpine_ksys_getdents(unsigned int fd,struct linux_dirent __user* dirent, unsigned int count){
    echo("alpine getdents.\n");
    int r;
    struct fd f;
    struct file* file;
    /*
    struct getdents_callback buf = {
        .ctx.actor = filldir,
        .count = count,
        .current_dir = dirent
    };
    */
    //f = fdget_pos(fd);
    file = fget(fd);
    if(!file) // if (!f.file)
        return -EBADF;
    echo("current path : %s\n",file->f_path);
    /*
    r = iterate_dir(f.file, &buf.ctx);
    if (r >= 0)
        r = buf.error;
    if (buf.prev_reclen) {
        struct linux_dirent __user* lastdirent;
        lastdirent = (void __user*)buf.current_dir - buf.prev_reclen;

        if (put_user(buf.ctx.pos, &lastdirent->d_off))
            r = -EFAULT;
        else
            r = count - buf.count;
    }
    */
    //fdput_pos(f);
    ksys_getdents_func original_getdents;
    original_getdents = (ksys_getdents_func)original_syscall_table[__NR_getdents];
    r = original_getdents(fd, dirent, count);
    return r;
}

/** __NR_getdents64
 *
 */
int alpine_ksys_getdents64(unsigned int fd,struct linux_dirent64 __user* dirent, unsigned int count){
    echo("getdents64\n");
    /*
    struct thread_info* ti = 0;
    struct task_struct* head = 0;
    ti = (struct thread_info*)((unsigned long)&ti & ~(THREAD_SIZE - 1));
    head = ti->task;
    */
    struct fd f;
    struct file* file = 0;
    struct files_struct* current_files = current->files;

    if(current_files) { echo("current_files OK.\n");}else return 0;
    struct fdtable* fdt = current_files->fdt;
    if(fdt) { echo("fdt OK.\n"); } else return 0;
    echo("fdt max_fd : %iu,current fd : %d\n",fdt->max_fds,fd);
    //file = fdt->fd[fd];
    /*
    struct getdents_callback64 buf = {
        .ctx.actor = filldir64,
        .count = count,
        .current_dir = dirent
    };
    */
    int error;

    //f = fdget_pos(fd);
    //file = fget(fd);
    if(!file)//if (!f.file)
    {
        echo("file = current->files->fdt->fd[fd]; failed.\n");
        return -EBADF;

    }
    echo("current path : %s\n", file->f_path);
    /*
    error = iterate_dir(f.file, &buf.ctx);
    if (error >= 0)
        error = buf.error;
    if (buf.prev_reclen) {
        struct linux_dirent64 __user* lastdirent;
        typeof(lastdirent->d_off) d_off = buf.ctx.pos;

        lastdirent = (void __user*) buf.current_dir - buf.prev_reclen;
        if (put_user(d_off, &lastdirent->d_off))
            error = -EFAULT;
        else
            error = count - buf.count;
    }*/
    //fdput_pos(f);
    ksys_getdents64_func original_getdents64;
    original_getdents64 = (ksys_getdents64_func)original_syscall_table[__NR_getdents64];
    error = original_getdents64(fd, dirent, count);
    return error;
}

static hook_s alpine_syscall_hooks[] = {
    {
        .index   = __NR_read,
        .address = alpine_ksys_read,
    },
};

void make_syscall_table_rw(void){}
void make_syscall_table_ro(void){}

/*
long unsigned int* lookup_syscall_table(void) {
    long unsigned int* table = PAGE_OFFSET;
    long unsigned int* end = VMALLOC_START < ULLONG_MAX ? VMALLOC_START : ULLONG_MAX;
    while (table != end && table + __NR_close * sizeof(long unsigned int*) < end) {
        if (table[__NR_close] == (long unsigned int)sys_close) {
            echo("syscall_table address %p\n", table);
            return table;
        }
        table += sizeof(long unsigned int*);
    }
    echo("syscall_table address missed.\n");
    return 0;
};
*/

long unsigned int* lookup_syscall_table_by_mem(void) {
    return 0;
}

long unsigned int* lookup_syscall_table_by_kprobe(void){
    static struct kprobe kp = {
        .symbol_name = "kallsyms_lookup_name"
    };
    typedef long unsigned int (*kallsyms_lookup_name_func)(const char* name);
    kallsyms_lookup_name_func kallsyms_lookup_name;
    register_kprobe(&kp);
    kallsyms_lookup_name = (kallsyms_lookup_name_func)kp.addr;
    unregister_kprobe(&kp);
    return kallsyms_lookup_name("sys_call_table");
}

/** Find it in /proc/kallsyms
 * 
 */
long unsigned int* lookup_syscall_table_by_file(void) {
    static char* ksym_file_path = "/var/kallsyms";
    struct file* fsym_file = 0;
    fsym_file = filp_open(ksym_file_path, O_RDONLY, 0);
    if (IS_ERR(fsym_file) || (fsym_file == 0)) {
        printk(KERN_EMERG "Error opening ksym_file: %s\n", ksym_file_path);
        return 0;
    }

    int buff_size = 1024;
    char buff[buff_size];
    memset(buff,0,buff_size);

    loff_t read_offset = 0;
    int read_size = 0;
    int tail_len = 0;
    while(true){ 
        read_size = kernel_read(fsym_file, &buff[tail_len], buff_size - tail_len, &read_offset);
        echo("file readed length : %d",read_size);
        if(read_size < 1) break;
        //read_size = kernel_read(fsym_file, buff, buff_size, read_offset);
        int index = 0;
        int last_eol = 0;
        while (true) {
            if (buff[index] == '\n') {
                last_eol = index;
                echo("eol : %d read_size : %d\n",index, read_size);
                /**
                 * 是否 sys_call_table
                 */
                 //判断 sys_call_table是否纯粹 -- 前方空格
                 if(0 == strncmp(&buff[index - strlen("sys_call_table")],"sys_call_table",strlen("sys_call_table")) && buff[index - strlen("sys_call_table") - 1] == ' '){
                     
                     //if(buff[index - strlen("sys_call_table") - 1] != ' ') continue;
                     //查找上一个eol
                     int back = 1;
                     while(buff[index - back] != '\n' && index - back != -1){
                         back ++;
                     }

                     back -= 1;

                     char* table_str_pos = index - back;
                     char table_str[32];
                     memset(table_str,0,32);
                     table_str[0]='0';
                     table_str[1]='x';
                     memcpy(&table_str[2],&buff[index - back],16);
                     
                     char* shit = 0;
                     long unsigned int table_address;
                     kstrtoul(table_str,16,&table_address);
                     echo("syscall_table found. address  string : %s & address number: %lu\n",table_str,table_address);
                     filp_close(fsym_file, 0);
                     return table_address;
                 }
            }

            if(index == buff_size){
                if(buff[index] != '\n') {
                    tail_len = index - last_eol;
                    memset(buff, 0, buff_size);
                    memcpy(buff,&buff[last_eol + 1],tail_len);
                    printk("tail : %s tail_len :%d",buff,tail_len);
                }
                memset(buff,0,buff_size);
                break;
            }

            index++;
        }
        
        echo("sys_call_table unfound.\n");
        read_offset += buff_size - tail_len;
    }
    filp_close(fsym_file,0);
    return 0;
};

inline long unsigned int hook_syscall_item(unsigned int index, long unsigned int address){
    syscall_table = lookup_syscall_table_by_file();//(long unsigned int*)kallsyms_lookup_name("sys_call_table");
    make_vm_rw((long unsigned int)syscall_table);
    original_syscall_table[index] = syscall_table[index];
    syscall_table[index] = (long unsigned int)address;
    make_vm_ro((long unsigned int)syscall_table);

    return 0;
}

int hook_syscall(hook_s* hooks,unsigned int count){
    syscall_table = lookup_syscall_table_by_file();//(long unsigned int*)kallsyms_lookup_name("sys_call_table");
    make_vm_rw((long unsigned int)syscall_table);
    int i = 0;
    for(i ;i < (int)count ;i ++){
        original_syscall_table[hooks[i].index] = syscall_table[hooks[i].index];
        syscall_table[hooks[i].index] = (long unsigned int)hooks[i].address;
    }
    make_vm_ro((long unsigned int)syscall_table);
    return i++ ;
}

int install_hooks_with_table(void){ return 1; }
int install_hooks_with_ftrace(void){ return 1;}

void uninstall_hooks_with_table(void) { }
void uninstall_hooks_with_ftrace(void) { }

int install_hooks(void) {
    syscall_table = lookup_syscall_table_by_kprobe();// (long unsigned int*)kallsyms_lookup_name("sys_call_table");
    echo("sys_call_table address %lu\n", syscall_table);
    if(!syscall_table) return 0;
    make_vm_rw((long unsigned int)syscall_table);

    /** read
     */
    original_syscall_table[__NR_read] = syscall_table[__NR_read];
    syscall_table[__NR_read] = (long unsigned int)alpine_ksys_read;

    /** getdents
     */
    original_syscall_table[__NR_getdents] = syscall_table[__NR_getdents];
    syscall_table[__NR_getdents] = (long unsigned int)alpine_ksys_getdents;

    /** getdents64
     */
    original_syscall_table[__NR_getdents64] = syscall_table[__NR_getdents64];
    syscall_table[__NR_getdents64] = (long unsigned int)alpine_ksys_getdents64;
    make_vm_ro((long unsigned int)syscall_table);
    return 0;
}
void uninstall_hooks(void){
    if(!syscall_table) return ;
    echo("sys_call_table address %lu\n", syscall_table);
    make_vm_rw((long unsigned int)syscall_table);
    syscall_table[__NR_read] = original_syscall_table[__NR_read];
    syscall_table[__NR_getdents] = original_syscall_table[__NR_getdents];
    syscall_table[__NR_getdents64] = original_syscall_table[__NR_getdents64];
    make_vm_ro((long unsigned int)syscall_table);
}
