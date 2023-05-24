#include "vfs.h"
#include "../../core/echo.h"

#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/kallsyms.h>

unsigned int vfs_open(){ return 0;}
EXPORT_SYMBOL(vfs_open);
unsigned int vfs_seek() { return 0; }
EXPORT_SYMBOL(vfs_seek);
unsigned int vfs_read() { return 0; }
EXPORT_SYMBOL(vfs_read);
unsigned int vfs_write() { return 0; }
EXPORT_SYMBOL(vfs_write);

int vfs_mkdir(){ return 0; }
EXPORT_SYMBOL(vfs_mkdir);
int vfs_rmdir(){ return 0; }
EXPORT_SYMBOL(vfs_rmdir);

int vfs_mklink(){ return 0; }
EXPORT_SYMBOL(vfs_mklink);
int vfs_rmlink(){ return 0; }
EXPORT_SYMBOL(vfs_rmlink);

int vfs_init(void) {
    
}

void vfs_exit(void){

}
