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
    echo("kernel_network_hooks inited.\n");
    unsigned long* sys_call_table;

    // 获取 sys_call_table 的虚拟内存地址
    sys_call_table = (unsigned long*)kallsyms_lookup_name("sys_call_table");
    echo("sys_call_table address %p\n", sys_call_table);
    cpp_on_init();
    return 0;
}

static void __exit this_module_exit(void)
{
    cpp_on_exit();
    echo("kernel_network_hooks exited.\n");
}

module_init(this_module_init);
module_exit(this_module_exit);
