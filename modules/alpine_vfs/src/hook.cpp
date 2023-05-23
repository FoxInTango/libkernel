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
class Hook {
public:
    //std::vector<int> hook_array;
public:
    Hook() { 
    echo("Hook Construct.\n"); 
    foxintango::Array<int> *int_array = new foxintango::Array<int>();

    if(int_array) { echo("foxintango::Array<int> allocated.\n");  delete int_array; }
    }
   ~Hook() { 
        echo("Hook Distruct.\n");
   }
};
static Hook* hook = 0;
void cpp_on_init(void) {
    hook = new Hook();
}
void cpp_on_exit(void){
    if(hook) delete hook;
}
