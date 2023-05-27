#ifndef _KERNEL_H_foxintango
#define _KERNEL_H_foxintango
#include <libcpp/libcpp.h>

EXTERN_C_BEGIN
namespaceBegin(foxintango)
class foxintangoAPI Kernel{
public:
    Kernel();
    ~Kernel();
public:
    int install(const char* path,const char* name = 0);
    int uninstall(const char* name);
};
namespaceEnd
EXTERN_C_END
#endif // !_KERNEL_H_foxintango
