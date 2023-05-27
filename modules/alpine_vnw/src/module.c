#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/kallsyms.h>
#include "hook.h"
#include "../../core/echo.h"

MODULE_AUTHOR("Dali Li");
MODULE_LICENSE("GPL");

//module_param(nbr,int,S_IRUGO);

static int __init this_module_init(void)          /* 加载模块时自动执行 */
{
    return 0;
}

static void __exit this_module_exit(void)
{
}

module_init(this_module_init);
module_exit(this_module_exit);
