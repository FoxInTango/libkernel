#define ALPINE_VFS_IMPLEMENT
#include "vfs.h"
#include "../../core/echo.h"

#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/kallsyms.h>

unsigned int alpine_vfs_open(void){ return 0;}
EXPORT_SYMBOL(alpine_vfs_open);
unsigned int alpine_vfs_seek(void) { return 0; }
EXPORT_SYMBOL(alpine_vfs_seek);
unsigned int alpine_vfs_read(void) { return 0; }
EXPORT_SYMBOL(alpine_vfs_read);
unsigned int alpine_vfs_write(void) { return 0; }
EXPORT_SYMBOL(alpine_vfs_write);

int alpine_vfs_mkdir(void){ return 0; }
EXPORT_SYMBOL(alpine_vfs_mkdir);
int alpine_vfs_rmdir(void){ return 0; }
EXPORT_SYMBOL(alpine_vfs_rmdir);

int alpine_vfs_mklink(void){ return 0; }
EXPORT_SYMBOL(alpine_vfs_mklink);
int alpine_vfs_rmlink(void){ return 0; }
EXPORT_SYMBOL(alpine_vfs_rmlink);

int alpine_vfs_init(void) {
    return 0;
}

void alpine_vfs_exit(void){

}
