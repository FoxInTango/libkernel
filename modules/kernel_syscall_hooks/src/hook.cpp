#include "hook.h"
#include "echo.h"

#include <vector>
#include <libcpp/libcpp.h>

class Hook {
public:
    std::vector<int> hook_array;
public:
    Hook() { echo("Hook Here.\n"); 
    foxintango::Array<int> int_array;
    
    for(int i = 0;i < 10;i ++) {
        //hook_array.push_back(i);
    }
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
