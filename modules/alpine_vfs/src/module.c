#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/kallsyms.h>
#include "hook.h"
#include "../../core/echo.h"

MODULE_AUTHOR("Dali Li");
MODULE_LICENSE("GPL");

static int __init this_module_init(void)
{
    return vfs_init();
}

static void __exit this_module_exit(void)
{
    vfs_exit();
}

module_init(this_module_init);
module_exit(this_module_exit);
