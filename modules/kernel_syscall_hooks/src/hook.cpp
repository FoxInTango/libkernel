#include "hook.h"
#include "echo.h"

#include <linux/kallsyms.h>
#include <vector>

class Hook {
public:
    std::vector<int> hook_array;
public:
    Hook() { echo("Hook Here.\n"); 
    for(int i = 0;i < 10;i ++) {
        //hook_array.push_back(i);
    }
    unsigned long* sys_call_table;

    // 获取 sys_call_table 的虚拟内存地址
    sys_call_table = (unsigned long*)kallsyms_lookup_name("sys_call_table");
    echo("sys_call_table address %p\n",sys_call_table);
    }
   ~Hook() { echo("Hook Exited.\n");
       int i = 0;
       std::vector<int>::iterator iter = hook_array.begin();
       while(iter != hook_array.end()){
	    echo("Hook Array Element %d\n",i);
	    i ++;
           iter ++;
       }
   }
};
static Hook* hook = 0;
void cpp_on_init(void) {
    hook = new Hook();
}
void cpp_on_exit(void){
    if(hook) delete hook;
}
