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
typedef long unsigned int hook_func_address;

typedef ssize_t (*ksys_read_func)(unsigned int fd, char __user* buf, size_t count);

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


/** Find it in /proc/kallsyms
 * 
 */
long unsigned int* lookup_syscall_table(void) {
    static char* ksym_file_path = "/var/kallsyms";
    struct file* fsym_file = 0;
    //mm_segment_t oldfs;
    //oldfs = get_fs();
    //set_fs(KERNEL_DS);
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
                    echo("tail length : %d\n", tail_len);
                    memset(buff, 0, buff_size);
                    memcpy(buff,&buff[last_eol + 1],tail_len);
                    echo("tail : %s",buff);
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
    syscall_table = lookup_syscall_table();//(long unsigned int*)kallsyms_lookup_name("sys_call_table");
    make_vm_rw((long unsigned int)syscall_table);
    original_syscall_table[index] = syscall_table[index];
    syscall_table[index] = (long unsigned int)address;
    make_vm_ro((long unsigned int)syscall_table);

    return 0;
}

int hook_syscall(hook_s* hooks,unsigned int count){
    syscall_table = lookup_syscall_table();//(long unsigned int*)kallsyms_lookup_name("sys_call_table");
    make_vm_rw((long unsigned int)syscall_table);
    int i = 0;
    for(i ;i < (int)count ;i ++){
        original_syscall_table[hooks[i].index] = syscall_table[hooks[i].index];
        syscall_table[hooks[i].index] = (long unsigned int)hooks[i].address;
    }
    make_vm_ro((long unsigned int)syscall_table);
    return i++ ;
}

int install_hooks(void) {
    syscall_table = lookup_syscall_table();// (long unsigned int*)kallsyms_lookup_name("sys_call_table");
    echo("sys_call_table address %lu\n", syscall_table);
    if(!syscall_table) return 0;
    make_vm_rw(syscall_table);
    original_syscall_table[__NR_read] = syscall_table[__NR_read];
    syscall_table[__NR_read] = (long unsigned int)alpine_ksys_read;
    make_vm_ro((long unsigned int)syscall_table);
    return 0;
}
void uninstall_hooks(void){
    if(!syscall_table) return ;
    echo("sys_call_table address %lu\n", syscall_table);
    make_vm_rw(syscall_table);
    syscall_table[__NR_read] = original_syscall_table[__NR_read];
    make_vm_ro((long unsigned int)syscall_table);
}
