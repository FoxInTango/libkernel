#include "hook.h"
#include "../../core/echo.h"

#include <libarguments/libarguments.h>
#include <libcpp/libcpp.h>

#include <linux/module.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/time.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/blkdev.h>
#include <linux/parser.h>
#include <linux/smp_lock.h>
#include <linux/buffer_head.h>
#include <linux/exportfs.h>
#include <linux/vfs.h>
#include <linux/random.h>
#include <linux/mount.h>
#include <linux/namei.h>
#include <linux/quotaops.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>

#include <vector>
class AlpineVFS {
private:
    file_system_type type;
public:
    AlpineVFS() {
    }
   ~AlpineVFS() {
   }
};

static AlpineVFS* alpine_vfs = 0;
int vfs_init(void) {
    alpine_vfs = new AlpineVFS();
}
void vfs_exit(void){
    if(alpine_vfs) delete hook;
}
