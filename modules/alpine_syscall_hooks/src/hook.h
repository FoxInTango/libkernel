#ifndef _HOOK_H_
#define _HOOK_H_

#ifdef __cplusplus
extern "C" {
#endif
typedef long unsigned int hook_func_address;
typedef struct  _hook_s {
    char* target;
    hook_func_address address;
}hook_s;

/** syscall hooks
 */

int  install_hooks(void);
void uninstall_hooks(void);

#ifdef __cplusplus
}
#endif

#endif
