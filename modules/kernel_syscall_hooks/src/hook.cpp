#include "hook.h"
#include "../../core/echo.h"

#include <vector>
#include <libarguments/libarguments.h>
#include <libcpp/libcpp.h>

class Hook {
public:
    //std::vector<int> hook_array;
public:
    Hook() { 
    echo("Hook Here.\n"); 
    foxintango::Array<int> *int_array = new foxintango::Array<int>();

    if(int_array) { echo("foxintango::Array<int> allocated.\n");  }
    }
   ~Hook() { 
        echo("Hook Exited.\n");
   }
};
static Hook* hook = 0;
void cpp_on_init(void) {
    hook = new Hook();
}
void cpp_on_exit(void){
    if(hook) delete hook;
}
