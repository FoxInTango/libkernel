#include "vfs.h"
#include "../../core/echo.h"

#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/kallsyms.h>

unsigned int alpine_vfs_open(){ return 0;}
EXPORT_SYMBOL(alpine_vfs_open);
unsigned int alpine_vfs_seek() { return 0; }
EXPORT_SYMBOL(alpine_vfs_seek);
unsigned int alpine_vfs_read() { return 0; }
EXPORT_SYMBOL(alpine_vfs_read);
unsigned int alpine_vfs_write() { return 0; }
EXPORT_SYMBOL(alpine_vfs_write);

int alpine_vfs_mkdir(){ return 0; }
EXPORT_SYMBOL(alpine_vfs_mkdir);
int alpine_vfs_rmdir(){ return 0; }
EXPORT_SYMBOL(alpine_vfs_rmdir);

int alpine_vfs_mklink(){ return 0; }
EXPORT_SYMBOL(alpine_vfs_mklink);
int alpine_vfs_rmlink(){ return 0; }
EXPORT_SYMBOL(alpine_vfs_rmlink);

int alpine_vfs_init(void) {
    
}

void alpine_vfs_exit(void){

}
