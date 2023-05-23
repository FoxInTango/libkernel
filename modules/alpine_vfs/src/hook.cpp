#include "hook.h"
#include "../../core/echo.h"

#include <libarguments/libarguments.h>
#include <libcpp/libcpp.h>

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
